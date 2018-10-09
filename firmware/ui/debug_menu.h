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

#ifndef OCF4_DEBUG_MENU_H_
#define OCF4_DEBUG_MENU_H_

#include <array>
#include "ui/ui.h"

namespace ocf4 {

class DebugMenu : public Menu {
public:
  DISALLOW_COPY_AND_ASSIGN(DebugMenu);
  DebugMenu() { }
  ~DebugMenu() { }

  static constexpr int kMaxDebugPages = 8;

  virtual void Init() final;
  virtual void Tick() final;
  virtual void HandleEvent(const Ui::EventType &) final;
  virtual void Draw(Display::Frame &) const final;
  virtual void SerialCommand(uint8_t) final;
  virtual void DebugView(Display::Frame &) const final;

  void AddPage(const char *title, Debuggable *contents);

private:

  struct MenuPage {
    const char *title;
    Debuggable *contents;
  };

  std::array<MenuPage, kMaxDebugPages> pages_;
  int num_pages_ = 0;
  int current_page_ = 0;
};

extern DebugMenu debug_menu;

}; // namespace ocf4

#endif // OCF4_DEBUG_MENU_H_
