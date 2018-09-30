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
// Main runtime for ocf4

#include "ocf4.h"
#include "drivers/adc.h"
#include "drivers/gpio.h"
#include "drivers/core_timer.h"
#include "drivers/dac.h"

#include "stm32x/stm32x_core.h"
#include "stm32x/stm32x_debug.h"

#include "display.h"

STM32X_CORE_DEFINE();

namespace ocf4 {
  GPIO gpio;
  Adc adc;
  Spi shared_spi;
  Display display{shared_spi};
  Dac dac{shared_spi};
  CoreTimer core_timer;

}; // namespace ocf4
using namespace ocf4;
using namespace stm32x;

extern "C" void CORE_TIMER_HANDLER() {
  if (!core_timer.Ticked())
    return;

  display.Flush();
  dac.Update();

  adc.Sample();
  adc.StartConversion();

  // Stuff happens here

  display.Update();
}

extern "C" void SysTick_Handler() {
  STM32X_CORE_TICK();
}

int main()
{
  STM32X_DEBUG_INIT();
  STM32X_CORE_INIT(F_CPU / kSysTickUpdate);

  display.Init();

  core_timer.Start(F_CPU / kCoreUpdate - 1);

  uint32_t frame_count = 0;
  while (true) {

    auto frame = display.BeginFrame();
    if (frame.valid()) {
      frame->drawFrame(0, 0, 128, 64);
      frame->drawStr(8, 8, "HELLO WORLD");

      frame->setPrintPos(8, 16);
      frame->printf("%lu", frame_count);

      frame->setPrintPos(8, 24);
      frame->printf("ADC0: %ld", adc.value(0));
      frame->setPrintPos(8, 32);
      frame->printf("ADC1: %ld", adc.value(1));

      ++frame_count;
    }
 }
}
