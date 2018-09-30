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
#include "drivers/spi.h"
#include "drivers/gpio.h"

namespace ocf4 {

// SPI1_TX : DMA2_Stream3, DMA2_Stream5

void Spi::Init()
{
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);

  SPI_InitTypeDef spi_init;
  spi_init.SPI_Direction = SPI_Direction_1Line_Tx;
  spi_init.SPI_Mode = SPI_Mode_Master;
  spi_init.SPI_DataSize = SPI_DataSize_8b;
  spi_init.SPI_CPOL = SPI_CPOL_High;
  spi_init.SPI_CPHA = SPI_CPHA_1Edge;
  spi_init.SPI_NSS = SPI_NSS_Soft;
  spi_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; // 84/x => max = 42MHz
  spi_init.SPI_FirstBit = SPI_FirstBit_MSB;
  spi_init.SPI_CRCPolynomial = 7;

  SPI_Init(SPI1, &spi_init);
  SPI_Cmd(SPI1, ENABLE);

  DMA_DeInit(DMA2_Stream3);
  DMA_InitTypeDef dma_init;
  dma_init.DMA_Channel = DMA_Channel_3;
  dma_init.DMA_PeripheralBaseAddr = (uint32_t)&SPI1->DR;
  dma_init.DMA_Memory0BaseAddr = 0;
  dma_init.DMA_DIR = DMA_DIR_MemoryToPeripheral;
  dma_init.DMA_BufferSize = 0;
  dma_init.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  dma_init.DMA_MemoryInc = DMA_MemoryInc_Enable;
  dma_init.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  dma_init.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  dma_init.DMA_Mode = DMA_Mode_Normal;
  dma_init.DMA_Priority = DMA_Priority_Medium;
  dma_init.DMA_FIFOMode = DMA_FIFOMode_Disable;
  dma_init.DMA_FIFOThreshold = DMA_FIFOThreshold_1QuarterFull;
  dma_init.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  dma_init.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_Init(DMA2_Stream3, &dma_init);
}

void Spi::Send(const void *data, size_t length, bool flush)
{
  const uint8_t *d = static_cast<const uint8_t *>(data);
  while (length--)
    Send8(*d++);

  if (flush) Flush();
}

void Spi::AsyncTransfer(const void *data, size_t length)
{
  DMA2_Stream3->M0AR = (uint32_t)data;
  DMA2_Stream3->NDTR = length;
  DMA2->LIFCR = DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3; // DMA_ClearFlag(DMA2_Stream3, );
  SPI1->CR2 |= SPI_I2S_DMAReq_Tx; // SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, ENABLE);
  DMA2_Stream3->CR |= DMA_SxCR_EN; // DMA_Cmd(DMA2_Stream3, ENABLE);
}

bool Spi::AsyncTransferComplete()
{
  if (DMA2->LISR & DMA_FLAG_TCIF3 /*DMA_GetFlagStatus(DMA2_Stream3, DMA_FLAG_TCIF3)*/) {
    while (SPI1->SR & SPI_SR_BSY) { }
    DMA2->LIFCR = DMA_FLAG_TCIF3 | DMA_FLAG_HTIF3; // DMA_ClearFlag(DMA2_Stream3, );
    SPI1->CR2 &= ~SPI_I2S_DMAReq_Tx; // SPI_I2S_DMACmd(SPI1, SPI_I2S_DMAReq_Tx, DISABLE);
    DMA2_Stream3->CR &= ~DMA_SxCR_EN; // DMA_Cmd(DMA2_Stream3, DISABLE);
    return true;
  } else {
    return false;
  }
}

}; // namespace ocf4
