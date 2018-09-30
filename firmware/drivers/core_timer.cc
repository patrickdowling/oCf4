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
// Core timer

#include "stm32x/stm32x_core.h"
#include "drivers/core_timer.h"

namespace ocf4 {

void CoreTimer::Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
}

void CoreTimer::Start(uint32_t period)
{
  TIM_TimeBaseInitTypeDef timer_init;
  timer_init.TIM_Prescaler = 0;
  timer_init.TIM_CounterMode = TIM_CounterMode_Up;
  timer_init.TIM_Period = period;
  timer_init.TIM_ClockDivision = TIM_CKD_DIV1;
  timer_init.TIM_RepetitionCounter = 0;
  TIM_InternalClockConfig(TIM1);
  TIM_TimeBaseInit(TIM1, &timer_init);

  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);  // 2.2 priority split.

  NVIC_InitTypeDef timer_interrupt;
  timer_interrupt.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
  timer_interrupt.NVIC_IRQChannelPreemptionPriority = 0;
  timer_interrupt.NVIC_IRQChannelSubPriority = 0;
  timer_interrupt.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&timer_interrupt);

  TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);  
  TIM_Cmd(TIM1, ENABLE);
}

}; // namespace ocf4
