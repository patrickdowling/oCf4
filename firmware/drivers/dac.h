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
// DAC8565 driver

#include "ocf4.h"
#include <array>
#include "drivers/spi.h"

namespace ocf4 {

class Dac {
public:
  DISALLOW_COPY_AND_ASSIGN(Dac);
  Dac(Spi &spi) : spi_(spi) { Init(); }
  ~Dac() { }

  static constexpr size_t kNumChannels = 4;

  void Update(const std::array<uint16_t, kNumChannels> &values);

private:

  Spi &spi_;

  void Init();

  void Send(const void *data, size_t length) {
    gpio.CS_DAC.Reset();
    spi_.Send(data, length, true);
    gpio.CS_DAC.Set();
  }

  std::array<uint8_t, 4 * kNumChannels> tx_buffer_;
};

}; // namespace ocf4
