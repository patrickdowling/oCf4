// Copyright 2018 Patrick Dowling
//
// Author: Patrick Dowling (pld@gurkenkiste.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// See http://creativecommons.org/licenses/MIT/ for more information.
//
// -----------------------------------------------------------------------------
// ADC driver

#include "drivers/adc.h"
#include "drivers/gpio.h"

// CV1 = PB0 = ADC1_IN8/ADC2_IN8
// CV2 = PA7 = ADC7_IN8/ADC2_IN7
// CV3 = PC5 = ADC1_IN15/ADC2_IN15
// CV4 = PC4 = ADC1_IN14/ADC2_IN14

// ADC1 = DMA2_Stream0, channel 0
// Theory:
// Using simulatenous mode to sample on ADC1/2 simultaneously; this might give
// more options for oversampling. Alternatively, use interleaved mode?

// ./f4_adc_tconv.py ADC_Prescaler_Div6 ADC_TwoSamplingDelay_10Cycles 2 144
// num_channels=2, 168MHz / 2 / 6 = 14.00MHz
// ADC_SampleTime_3Cycles = 538.46KHz
// ADC_SampleTime_15Cycles = 280.00KHz
// ADC_SampleTime_28Cycles = 184.21KHz
// ADC_SampleTime_56Cycles = 106.06KHz
// ADC_SampleTime_84Cycles = 74.47KHz
// ADC_SampleTime_112Cycles = 57.38KHz
// ADC_SampleTime_144Cycles = 45.45KHz
// ADC_SampleTime_480Cycles = 14.29KHz
//
// Requested: 144.00KHz -> 184.21KHz
#define ADC_PRESCALER ADC_Prescaler_Div6
#define ADC_SAMPLE_TIME ADC_SampleTime_28Cycles
#define ADC_TWOSAMPLING_DELAY ADC_TwoSamplingDelay_10Cycles

// 144 = 2 channels @ 24KHz x 6x oversampling
// TODO Better fit with opamp settling time (33k/560p)

namespace ocf4 {

static const struct {
  ADC_TypeDef *adc;
  uint8_t adc_channel;
  uint8_t rank;
  uint8_t sample_time;
} adc_channel_config[Adc::kNumChannels] = {
  { ADC1, ADC_Channel_15, 2, ADC_SAMPLE_TIME },
  { ADC2, ADC_Channel_14, 2, ADC_SAMPLE_TIME },
  { ADC1, ADC_Channel_8, 1, ADC_SAMPLE_TIME },
  { ADC2, ADC_Channel_7, 1, ADC_SAMPLE_TIME },
};

void Adc::Init()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  ADC_CommonInitTypeDef adc_common_init;
  adc_common_init.ADC_Mode = ADC_DualMode_Interl; //ADC_DualMode_RegSimult;
  adc_common_init.ADC_Prescaler = ADC_PRESCALER;
  adc_common_init.ADC_DMAAccessMode = ADC_DMAAccessMode_2;
  adc_common_init.ADC_TwoSamplingDelay = ADC_TWOSAMPLING_DELAY;
  ADC_CommonInit(&adc_common_init);

  ADC_InitTypeDef adc_init;
  adc_init.ADC_Resolution = ADC_Resolution_12b;
  adc_init.ADC_ScanConvMode = ENABLE;
  adc_init.ADC_ContinuousConvMode = DISABLE;
  adc_init.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  adc_init.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
  adc_init.ADC_DataAlign = ADC_DataAlign_Right;
  adc_init.ADC_NbrOfConversion = kNumChannels * kOversampling / kNumAdcs;
  ADC_Init(ADC1, &adc_init);
  ADC_Init(ADC2, &adc_init);

  for (uint32_t i = 0; i < kOversampling; ++i) {
    for (auto &channel : adc_channel_config)
      ADC_RegularChannelConfig(
          channel.adc,
          channel.adc_channel,
          channel.rank + i * kNumChannels / kNumAdcs,
          channel.sample_time);
  }

  DMA_InitTypeDef dma_init;
  dma_init.DMA_Channel = DMA_Channel_0;
  dma_init.DMA_PeripheralBaseAddr = (uint32_t)&ADC->CDR;
  dma_init.DMA_Memory0BaseAddr = (uint32_t)raw_values_.data();
  dma_init.DMA_DIR = DMA_DIR_PeripheralToMemory;
  dma_init.DMA_BufferSize = kNumChannels * kOversampling / kNumAdcs;
  dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Word;
  dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
  dma_init.DMA_Mode = DMA_Mode_Circular;
  dma_init.DMA_Priority = DMA_Priority_High;
  dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
  dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream0, &dma_init);
  DMA_Cmd(DMA2_Stream0, ENABLE);

  ADC_MultiModeDMARequestAfterLastTransferCmd(ENABLE);
  ADC_DMACmd(ADC1, ENABLE);

  ADC_Cmd(ADC1, ENABLE);
  ADC_Cmd(ADC2, ENABLE);

  StartConversion();
}

void Adc::StartConversion()
{
  ADC_SoftwareStartConv(ADC1);
}

}; // namespace ocf4
