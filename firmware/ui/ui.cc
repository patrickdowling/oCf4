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
// Main UI Handling

#include "ui/ui.h"
#include "ui/debug_menu.h"

namespace ocf4 {

static constexpr ControlID encoders[] = { CONTROL_ENC_L, CONTROL_ENC_R };
static constexpr ControlID switches[] = { CONTROL_SWITCH_UP, CONTROL_SWITCH_DOWN, CONTROL_SWITCH_L, CONTROL_SWITCH_R };

void Ui::Init()
{
  events_.Push(UI::EVENT_NONE, CONTROL_DUMMY, 0, 0U);
  debug_menu.Init();
  active_menu_ = &debug_menu;
}

void Ui::Poll()
{
  encoders_.Poll();
  switches_.Poll();

  for (auto encoder : encoders) {
    int32_t increment = encoders_.get_increment(encoder - CONTROL_ENC_L);
    if (increment)
      events_.Push(UI::EVENT_ENCODER, encoder, increment, 0U);
  }

  for (auto sw : switches) {
    if (switches_[sw].just_pressed())
      events_.Push(UI::EVENT_BUTTON_PRESS, sw, 0, 0U);
  }
}

bool Ui::HandleEvent(const EventType &event)
{
  (void)event;
  return false;
}

}; // namespace ocf4
