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

#include "builtin/debug_menu.h"
#include "ocf4.h"
#include "io_frame.h"

namespace ocf4 {

void DebugMenu::Init()
{
  AddPage("SYS", this);
}

void DebugMenu::AddPage(const char *title, Debuggable *contents)
{
  if (num_pages_ < kMaxDebugPages) {
    pages_[num_pages_] = { title, contents };
    ++num_pages_;
  }
}

void DebugMenu::Tick()
{
}

void DebugMenu::HandleEvent(const EventType &event)
{
  if (UI::EVENT_ENCODER == event.type) {
    if (CONTROL_ENC_L == event.id) {
      current_page_ = current_page_ + event.value;
      CONSTRAIN(current_page_, 0, num_pages_ - 1);
    } else if (CONTROL_ENC_R == event.id) {
    }
  }
}

void DebugMenu::Draw(Display::Frame &frame) const
{
  const auto &current_page = pages_[current_page_];

  frame->printf(0, 0, "%d/%d %s", current_page_ + 1, num_pages_, current_page.title);
  frame->drawHLine(0, 10, 128);
  current_page.contents->DebugView(frame);
}

void DebugMenu::SerialCommand(uint8_t)
{
}

void DebugMenu::DebugView(Display::Frame &frame) const
{
  frame->printf(0, 16, "GFX  %lu", DEBUG_STATS.GFX.frame_count);
  frame->printf(0, 24, "UI   %lu", DEBUG_STATS.UI.event_count);

  auto core_timer_cycles = DEBUG_STATS.CORE.core_timer_cycles.value_in_us();
  frame->printf(0, 32, "CORE %3luus %.1f%%", core_timer_cycles, static_cast<float>(core_timer_cycles * 100) / static_cast<float>(kCoreUpdateTimeUs));
  frame->printf(0, 40, "FPS  %.2f", DEBUG_STATS.GFX.fps);
}

}; // namespace ocf4
