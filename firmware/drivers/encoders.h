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
// Encoder driver

#ifndef OCF4_DRIVERS_ENCODERS_H_
#define OCF4_DRIVERS_ENCODERS_H_

#include "util/util_macros.h"

namespace ocf4 {

static constexpr size_t kNumEncoders = 2;

class Encoders {
public:
  DISALLOW_COPY_AND_ASSIGN(Encoders);
  Encoders() { Init(); }
  ~Encoders() { }

  void Poll();

  int32_t get_increment(size_t index) const {
    return encoder_state_[index].increment;
  }

private:
  void Init();

  struct EncoderState {
    uint8_t pin_state[2];
    int32_t increment;

    void Update(bool pin_a, bool pin_b);
  };

  EncoderState encoder_state_[kNumEncoders];
};

}; // namespace ocf4

#endif // OCF4_DRIVERS_ENCODERS_H_
