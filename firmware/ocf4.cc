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
#include "calibration_data.h"
#include "display.h"
#include "patch.h"
#include "drivers/adc.h"
#include "drivers/digital_inputs.h"
#include "drivers/gpio.h"
#include "drivers/core_timer.h"
#include "drivers/dac.h"
#include "drivers/serial_port.h"
#include "stm32x/stm32x_core.h"
#include "stm32x/stm32x_debug.h"
#include "ui/ui.h"
#include "builtin/calibration_patch.h"
#include "builtin/debug_menu.h"
#include "builtin/io_frame_debug.h"

namespace ocf4 {

  DebugStats DEBUG_STATS;
  CalibrationData calibration_data;

  GPIO gpio;
  Adc adc;
  CoreTimer core_timer;
  DigitalInputs digital_inputs;
  Spi shared_spi;
  Display display{shared_spi};
  Dac dac{shared_spi};
  SerialPort serial_port;

  Ui ui;
  DebugMenu debug_menu;

}; // namespace ocf4
STM32X_CORE_DEFINE();

using namespace ocf4;
using namespace stm32x;

static PatchMemoryPool patch_memory_pool;
static Patch current_patch{patch_memory_pool};
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
  ++io_frame.tick;

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
  static constexpr uint32_t type_id = FOURCC<'T','E','S','T'>::value;
  virtual void Init(PatchMemoryPool &) final { }
  virtual void Process(IOFrame &frame) final {
    frame.out[0] = a++;
    frame.out[1] = b++;
  }

private:
  uint16_t a = 0, b = 0xffff/4, c = 0xffff/2, d = 0xffff*3 / 4;
};

class TestProcGate : public Processor {
public:
  static constexpr uint32_t type_id = FOURCC<'G','A','T','E'>::value;

  TestProcGate(uint32_t pw) {
    pw_.set(pw);
  }

  virtual void Init(PatchMemoryPool &) final { }
  virtual void Process(IOFrame &frame) final {
    if (ticks_ > pw_.value()) {
      frame.out[2] = 65535;
      frame.out[3] = 0;
    } else {
      frame.out[2] = 0;
      frame.out[3] = 65535;
    }
    ++ticks_;
    if (ticks_ > kCoreUpdate)
      ticks_ = 0;
  }

  ParameterList parameter_list() const {
    return ParameterList(parameters_.size(), parameters_.data());
  }

private:
  uint32_t ticks_ = 0;
  EditableParameter<uint32_t> pw_ = {"PW", kCoreUpdate / 2};

  const std::array<EditableParameterBase*, 2> parameters_ = {
    &pw_,
    nullptr
  };
};

int main()
{
  STM32X_DEBUG_INIT();
  STM32X_CORE_INIT(F_CPU / kSysTickUpdate);

  // FIXME Init order smells suspicious

  debug_menu.Init();
  debug_menu.AddPage("IO", &io_frame_debug);
  debug_menu.AddPage("PATCH", &current_patch);
  ui.PushMenu(&debug_menu);

  display.AdjustOffset(calibration_data.display_offset);
  display.Init();
/*
  CalibrationPatch::Load(current_patch);
  ui.PushMenu(current_patch.root_menu());
*/

  current_patch.Reset();
  current_patch.AddProcessor<TestProc>();
  current_patch.AddProcessor<TestProcGate>(kCoreUpdate / 2);
  current_patch.enable(true);

  serial_port.Init(9600);

  core_timer.Start(F_CPU / kCoreUpdate - 1);
  while (true) {
    ui.DispatchEvents();
    current_patch.IdleLoop();
    ui.Draw();

    while (serial_port.readable()) {
      switch (serial_port.Read()) {
        case '?': serial_port.Write('!');
        default: break;
      }
    }
 }
}
