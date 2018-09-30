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

#ifndef OCF4_DRIVERS_CORE_TIMER_H_
#define OCF4_DRIVERS_CORE_TIMER_H_

#include "stm32x/stm32x_core.h"

#define CORE_TIMER_HANDLER TIM1_UP_TIM10_IRQHandler
//extern "C" void CORE_TIMER_HANDLER() INRAM;

namespace ocf4 {

class CoreTimer {
public:
  CoreTimer() { Init(); };
  ~CoreTimer() { };

  void Start(uint32_t period);

  inline bool Ticked() const ALWAYS_INLINE {
    if (TIM1->SR & TIM_IT_Update) {
      TIM1->SR = (uint16_t)~TIM_IT_Update;
      return true;
    } else {
      return false;
    }
  }

private:
  void Init();
};

}; // namespace ocf4

#endif // OCF4_DRIVERS_CORE_TIMER_H_
