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

#ifndef OCF4_UI_H_
#define OCF4_UI_H_

#include "ocf4.h"
#include "display.h"
#include "drivers/encoders.h"
#include "drivers/switches.h"
#include "ui/ui_event_queue.h"

namespace ocf4 {

class Menu;
class Debuggable;

enum ControlID : uint16_t {
  CONTROL_SWITCH_UP,
  CONTROL_SWITCH_DOWN,
  CONTROL_SWITCH_L,
  CONTROL_SWITCH_R,
  CONTROL_ENC_L,
  CONTROL_ENC_R,
  CONTROL_DUMMY
};

static constexpr size_t kUiMenuStackDepth = 8;
static constexpr size_t kUiEventQueueDepth = 16;
using EventQueue = UI::EventQueue<ControlID, kUiEventQueueDepth>;
using EventType = EventQueue::EventType;

class Ui {
public:
  DISALLOW_COPY_AND_ASSIGN(Ui);
  Ui () { Init(); };
  ~Ui() { }

  void Poll();
  void Tick();
  void DispatchEvents();
  void Draw();

  void PushMenu(Menu *menu) {
    if (stacked_menus_ < kUiMenuStackDepth)
      menu_stack_[stacked_menus_++] = menu;
  }

  void PopMenu() {
    if (stacked_menus_) {
      --stacked_menus_;
      menu_stack_[stacked_menus_] = nullptr;
    }
  }

private:
  Encoders encoders_;
  Switches switches_;
  EventQueue events_;

  std::array<Menu *, kUiMenuStackDepth> menu_stack_{nullptr};
  size_t stacked_menus_ = 0;

  bool screensaver_active_ = false;

  void Init();
  bool HandleEvent(const EventType &event);

  inline Menu *get_active_menu() {
    if (stacked_menus_)
      return menu_stack_[stacked_menus_ - 1];
    else
      return nullptr;
  }
};

extern Ui ui;
} // namespace ocf4

#endif // OCF4_UI_H_
