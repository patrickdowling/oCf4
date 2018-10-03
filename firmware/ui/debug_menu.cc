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

#include "debug_menu.h"
#include "ocf4.h"
#include "io_frame.h"

namespace ocf4 {

DebugMenu debug_menu;

void DebugMenu::Init()
{
}

void DebugMenu::HandleEvent(const Ui::EventType &)
{
}

void DebugMenu::Draw(Display::Frame &frame)
{
  frame->setPrintPos(0, 0);
  frame->printf("OCF4");
  frame->drawHLine(0, 10, 128);

  frame->setPrintPos(0, 16);
  frame->printf("GFX  %lu", DEBUG_STATS.GFX.frame_count);

  frame->setPrintPos(0, 24);
  frame->printf("UI   %lu", DEBUG_STATS.UI.event_count);

  frame->setPrintPos(0, 32);
  auto core_timer_cycles = DEBUG_STATS.CORE.core_timer_cycles.value_in_us();
  frame->printf("CORE %3luus %.1f%%", core_timer_cycles, static_cast<float>(core_timer_cycles * 100) / static_cast<float>(kCoreUpdateTimeUs));

  frame->setPrintPos(0, 40);
  frame->printf("FPS  %.2f", DEBUG_STATS.GFX.fps);
/*
  frame->setPrintPos(8, 24);
  for (const auto &i : io_frame.digital_inputs)
    frame->printf("   %c ", i.high() ? 'H' : '_' );

  frame->setPrintPos(0, 32);
  frame->printf("%4ld %4ld %4ld %4ld", io_frame.adc_in[0], io_frame.adc_in[1], io_frame.adc_in[2], io_frame.adc_in[3]);
*/
}

void DebugMenu::SerialCommand(uint8_t)
{
}

}; // namespace ocf4
