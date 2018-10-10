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
#include <limits>
#include <util/util_macros.h>

namespace ocf4 {

class Adc {
public:
  DISALLOW_COPY_AND_ASSIGN(Adc);
  Adc() { Init(); }
  ~Adc() { }

  static constexpr size_t kNumChannels = 4;
  static constexpr size_t kNumAdcs = 2;
  static constexpr uint32_t kOversampling = 6;
  static_assert(kNumChannels/kNumAdcs * kOversampling <= 16, "Max number of conversions per ADC channel");

  inline void Read(std::array<int32_t, kNumChannels> &values) const {
    auto src = std::begin(raw_values_);
    auto dst = std::begin(values);
    for (size_t i = 0; i < kNumChannels; ++i) {
      *dst++ = SampleValue(src++);

    }
    /*
    std::transform(
        std::begin(raw_values_), std::begin(raw_values_) + kNumChannels, // std::end(raw_values_),
        std::begin(values), [](uint16_t v) { return v >> 4; });
    */
  }

  void StartConversion();

private:
  std::array<uint16_t, kNumChannels * kOversampling> raw_values_;

  void Init();

  // Vague attempts to tame the F4 ADC noisiness.
  // - Oversampling
  // - Discard largest and smallest sample
  template <typename iterator>
  constexpr int32_t SampleValue(iterator it) const {
    int32_t sum = 0;
    int32_t min_sample = std::numeric_limits<uint16_t>::max();
    int32_t max_sample = std::numeric_limits<uint16_t>::min();
    for (size_t i = 0; i < kOversampling; ++i, it += kNumChannels) {
      int32_t sample = *it;
      sum += sample;
      if (sample < min_sample) min_sample = sample; // std::min(sample, min);
      if (sample > max_sample) max_sample = sample; // std::max(sample, max);
    }
    return (sum - min_sample - max_sample) / (kOversampling - 2);
  }

};

}; // namespace ocf4

#endif // OCF4_DRIVERS_ADC_H_
