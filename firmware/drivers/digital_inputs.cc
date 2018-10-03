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
// Digital input drivers

#include "drivers/digital_inputs.h"

// GPIO_AF2_TIM3 > TR2; CH1 PC6
// GPIO_AF2_TIM3 > TR1; CH2 PC7
// GPIO_AF2_TIM3 > TR3; CH3 PC8
// GPIO_AF2_TIM3 > TR4; CH4 PC9

namespace ocf4 {

static const uint16_t tim_channels[] = { TIM_Channel_1, TIM_Channel_2, TIM_Channel_3, TIM_Channel_4 };

void DigitalInputs::Init()
{
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

  TIM_TimeBaseInitTypeDef timer_init;
  timer_init.TIM_Prescaler = 0;
  timer_init.TIM_CounterMode = TIM_CounterMode_Up;
  timer_init.TIM_Period = 0xffff;
  timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
  timer_init.TIM_RepetitionCounter = 0;
  TIM_InternalClockConfig(TIM3);
  TIM_TimeBaseInit(TIM3, &timer_init);

  TIM_ICInitTypeDef tim_ic_init;
  tim_ic_init.TIM_Channel = TIM_Channel_2;
  tim_ic_init.TIM_ICPolarity = TIM_ICPolarity_Falling;
  tim_ic_init.TIM_ICSelection = TIM_ICSelection_DirectTI;
  tim_ic_init.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  tim_ic_init.TIM_ICFilter = 0x0; // TODO?

  for (uint16_t tim_channel : tim_channels) {
    tim_ic_init.TIM_Channel = tim_channel;
    TIM_ICInit(TIM3, &tim_ic_init);
  }

  TIM_Cmd(TIM3, ENABLE);
}

// When an input capture occurs:
// * The TIMx_CCR1 register gets the value of the counter on the active transition.
// * CC1IF flag is set (interrupt flag).
// CC1OF is also set if at least two consecutive captures occurred where the flag was not cleared.
// NOTE We could read TIM3->SR once and process it "off-line", similarly read GPIOC->6789 at once
// NOTE Race conditions? Unlikely
// NOTE There should be bit-banded addresses for these ports
//
// It seems to make sense to check/clear the overflow flag first and reduce
// the race conditions that would lead to loss of detection. Any additional
// edges that arrive before the CCR is read would then at least be counted.
static inline bool GetCapture(TIM_TypeDef *tim, uint16_t channel) {
  const uint32_t sr = tim->SR;
  bool rising_edge = false;
  if (sr & static_cast<uint16_t>(TIM_FLAG_CC1OF << (channel - 1))) {
    tim->SR = static_cast<uint16_t>(~(TIM_FLAG_CC1OF << (channel - 1)));
    rising_edge = true;
  }
  if (sr & static_cast<uint16_t>(TIM_FLAG_CC1 << (channel - 1))) {
    //(void)*(&tim->CCR1 + (channel - 1)); // cleared by software or by reading the TIMx_CCRx register.
    tim->SR = static_cast<uint16_t>(~(TIM_FLAG_CC1 << (channel - 1)));
    rising_edge = true;
  }
  return rising_edge;
}

void DigitalInputs::Read(std::array<DigitalInputState, kNumChannels> &digital_input_state)
{
  digital_input_state[0].Update(GetCapture(TIM3, 2) || !gpio.TR1.Read());
  digital_input_state[1].Update(GetCapture(TIM3, 1) || !gpio.TR2.Read());
  digital_input_state[2].Update(GetCapture(TIM3, 3) || !gpio.TR3.Read());
  digital_input_state[3].Update(GetCapture(TIM3, 4) || !gpio.TR4.Read());
}

}; // namespace ocf4
