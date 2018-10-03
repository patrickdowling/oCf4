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

#include "drivers/encoders.h"
#include "drivers/gpio.h"

static constexpr uint8_t kPinMask = 0x03;
static constexpr uint8_t kPinEdge = 0x02;

namespace ocf4 {

void Encoders::EncoderState::Update(bool pin_a, bool pin_b)
{
  uint8_t pin_state_a = pin_state[0] << 1 | pin_a;
  uint8_t pin_state_b = pin_state[1] << 1 | pin_b;

  int32_t i = 0;
  // Find direction by detecting state change and evaluating the other pin.
  // 0x02 == b10 == rising edge on pin
  // Should also work just checking for falling edge on PINA and checking
  // PINB state.
  const uint8_t a = pin_state_a & kPinMask;
  const uint8_t b = pin_state_b & kPinMask;
  if (a == kPinEdge && b == 0x00) {
    i = 1;
  } else if (b == kPinEdge && a == 0x00) {
    i = -1;
  }

  // TODO Accleration from o_C
  pin_state[0] = pin_state_a;
  pin_state[1] = pin_state_b;
  increment = i;
}


void Encoders::Init()
{
  for (auto &state : encoder_state_) {
    state.pin_state[0] = state.pin_state[1] = 0xff;
    state.increment = 0;
  }
}

void Encoders::Poll()
{
  encoder_state_[0].Update(gpio.ENC_R1.Read(), gpio.ENC_R2.Read());
  encoder_state_[1].Update(gpio.ENC_L1.Read(), gpio.ENC_L2.Read());
}

}; // namespace ocf4
