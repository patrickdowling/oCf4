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
// ADC driver

#ifndef OCF4_DRIVERS_ADC_H_
#define OCF4_DRIVERS_ADC_H_

#include <array>
#include <algorithm>
#include <util/util_macros.h>

namespace ocf4 {

class Adc {
public:
  DISALLOW_COPY_AND_ASSIGN(Adc);
  Adc() { Init(); }
  ~Adc() { }

  static constexpr size_t kNumChannels = 4;

  void Sample() {
    std::copy(std::begin(raw_values_), std::end(raw_values_), std::begin(values_));
  }

  int32_t value(size_t channel) const {
    return values_[channel];
  }

  void StartConversion();

private:
  void Init();

  std::array<uint16_t, kNumChannels> raw_values_;
  std::array<int32_t, kNumChannels> values_;
};

}; // namespace ocf4

#endif // OCF4_DRIVERS_ADC_H_
