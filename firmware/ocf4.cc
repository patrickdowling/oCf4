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
#include "display.h"
#include "drivers/adc.h"
#include "drivers/digital_inputs.h"
#include "drivers/gpio.h"
#include "drivers/core_timer.h"
#include "drivers/dac.h"
#include "stm32x/stm32x_core.h"
#include "stm32x/stm32x_debug.h"
#include "ui/ui.h"
#include "ui/debug_menu.h"

namespace ocf4 {
  DebugStats DEBUG_STATS;

  GPIO gpio;
  Adc adc;
  CoreTimer core_timer;
  DigitalInputs digital_inputs;
  Spi shared_spi;
  Display display{shared_spi};
  Dac dac{shared_spi};
}; // namespace ocf4
STM32X_CORE_DEFINE();

using namespace ocf4;
using namespace stm32x;

static IOFrame io_frame;
extern "C" void CORE_TIMER_HANDLER() {
  if (!core_timer.Ticked())
    return;

  DEBUG_PROFILE_SCOPE(DEBUG_STATS.CORE.core_timer_cycles);
  display.Flush();
  dac.Update(io_frame.out);
  adc.Read(io_frame.adc_in);
  adc.StartConversion();
  digital_inputs.Read(io_frame.digital_inputs);
  display.Update();

  // Stuff happens here
}

extern "C" void SysTick_Handler() {
  STM32X_CORE_TICK();
  ui.Poll();
}

int main()
{
  STM32X_DEBUG_INIT();
  STM32X_CORE_INIT(F_CPU / kSysTickUpdate);

  display.Init();
  debug_menu.Init();

  Menu *active_menu = &debug_menu;

  core_timer.Start(F_CPU / kCoreUpdate - 1);
  while (true) {
    ui.DispatchEvents(active_menu);
    ui.Draw(active_menu);
 }
}
