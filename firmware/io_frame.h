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

#ifndef OCF4_IO_FRAME_H_
#define OCF4_IO_FRAME_H_

#include "ocf4.h"
#include <array>

namespace ocf4 {

static constexpr size_t kNumChannels = 4;

class DigitalInputState {
public:
  void Update(bool raised) {
    uint8_t state = (state_ << 1) & RAISED_MASK;
    if (raised)
      state |= RISING_EDGE;
    state_ = state;
  }

  inline bool rising_edge() const {
    return state_ == RISING_EDGE;
  }

  inline bool falling_edge() const {
    return state_ == FALLING_EDGE;
  }

  inline bool high() const {
    return state_ & RISING_EDGE;
  }

private:
  enum State : uint8_t {
    LOW          = 0x0, // __
    RISING_EDGE  = 0x1, // _-
    FALLING_EDGE = 0x2, // -_
    RAISED_MASK  = 0x3  // xx
  };

  uint8_t state_ = 0;
};

struct IOFrame {
  uint32_t tick = 0;
  std::array<DigitalInputState, kNumChannels> digital_inputs;
  std::array<int32_t, kNumChannels> adc_in;
  std::array<uint16_t, kNumChannels> out;
};

}; // namespace ocf4

#endif // OCF4_IO_FRAME_H_
