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
#include "patch.h"
#include "drivers/adc.h"
#include "drivers/digital_inputs.h"
#include "drivers/gpio.h"
#include "drivers/core_timer.h"
#include "drivers/dac.h"
#include "stm32x/stm32x_core.h"
#include "stm32x/stm32x_debug.h"
#include "ui/ui.h"
#include "ui/debug_menu.h"
#include "ui/io_frame_debug.h"

namespace ocf4 {

  DebugStats DEBUG_STATS;

  GPIO gpio;
  Adc adc;
  CoreTimer core_timer;
  DigitalInputs digital_inputs;
  Spi shared_spi;
  Display display{shared_spi};
  Dac dac{shared_spi};

  DebugMenu debug_menu;
  Ui ui;

}; // namespace ocf4
STM32X_CORE_DEFINE();

using namespace ocf4;
using namespace stm32x;

static Patch current_patch;
static IOFrame io_frame;
static IOFrameDebug io_frame_debug{io_frame};

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
  if (current_patch.enabled())
    current_patch.Process(io_frame);
}

extern "C" void SysTick_Handler() {
  STM32X_CORE_TICK();
  ui.Poll();
  ui.Tick();
}

class TestProc : public Processor {
public:
  static constexpr uint32_t type_id = FOURCC<'T', 'E', 'S', 'T'>::value;
  virtual void Init(PatchMemoryPool &) final { }
  virtual void Process(IOFrame &) final { }
};

int main()
{
  STM32X_DEBUG_INIT();
  STM32X_CORE_INIT(F_CPU / kSysTickUpdate);

  debug_menu.AddPage("IO", &io_frame_debug);
  debug_menu.AddPage("PATCH", &current_patch);

  display.Init();
  current_patch.Reset();
  current_patch.AddProcessor<TestProc>();
  current_patch.enable(true);

  core_timer.Start(F_CPU / kCoreUpdate - 1);
  while (true) {
    ui.DispatchEvents();
    ui.Draw();
 }
}
