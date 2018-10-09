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
#ifndef OCF4_H_
#define OCF4_H_

#include <stdint.h>
#include "stm32x/stm32x_debug.h"

namespace ocf4 {

static constexpr uint32_t kSysTickUpdate = 1000UL;
static constexpr uint32_t kCoreUpdate = 24000UL;
static constexpr uint32_t kCoreUpdateTimeUs = (1000000UL / kCoreUpdate);

static constexpr size_t kNumProcessorSlots = 4;
static constexpr size_t kPatchPoolSize  = 64*1024;


struct DebugStats {
  struct {
    stm32x::AveragedCycles core_timer_cycles;
  } CORE;

  struct {
    uint32_t frame_count = 0;
    float fps = 0.f;
  } GFX;

  struct {
    uint32_t event_count = 0;
  } UI;
};
extern DebugStats DEBUG_STATS;

}; // namespace ocf4

#ifdef OCF4_ENABLE_PROFILE
#define DEBUG_PROFILE_SCOPE(x) stm32x::ScopedCycleMeasurement debug_profile_scope{x}
#else
#define DEBUG_PROFILE_SCOPE(x) do {} while (false)
#endif

#endif // OCF4_H_
