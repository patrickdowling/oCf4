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
#ifndef UI_EVENT_QUEUE_H_
#define UI_EVENT_QUEUE_H_

#include "util/util_ringbuffer.h"
#include "ui/ui_event.h"
#include "stm32x/stm32x_core.h"

namespace UI {

template <typename control_id, size_t size>
class EventQueue {
public:
  DISALLOW_COPY_AND_ASSIGN(EventQueue);
  EventQueue () { Init(); };
  ~EventQueue() { }

  using EventType = Event<control_id>;

  void Init() {
    events_.Init();
  }

  inline void Flush() {
    events_.Flush();
  }

  inline bool empty() const {
    return !events_.readable();
  }

  inline EventType Pop() {
    return events_.Read();
  }

  template <class... Args>
  inline void Push(Args&&... args) {
    events_.EmplaceWrite(args...);
    last_event_time_ = STM32X_CORE_NOW();
  }

  inline void Poke() {
    last_event_time_ = STM32X_CORE_NOW();
  }

  inline uint32_t idle_time() const {
    return STM32X_CORE_NOW() - last_event_time_;
  }

private:

  util::RingBuffer<EventType, size> events_;
  uint32_t last_event_time_ = 0;
};

} // namespace UI

#endif // UI_EVENT_QUEUE_H_
