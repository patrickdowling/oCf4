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
// Display/framebuffer management

// Could also be specialized for fixed number of frames.

#ifndef GFX_FRAMEBUFFER_H_
#define GFX_FRAMEBUFFER_H_

#include <cstring>
#include "util/util_macros.h"
#include "gfx/weegfx.h"

// TODO Smart frame reference that automatically uses read/written in destructor

namespace weegfx {

template <typename DisplayType, size_t num_frames = 2>
class Framebuffer {
public:
  DISALLOW_COPY_AND_ASSIGN(Framebuffer);
  Framebuffer() { Init(); };
  ~Framebuffer() { }

  static constexpr size_t kFrameSize = DisplayType::kFrameSize;
  static constexpr size_t kNumFrames = num_frames;

  void Init() {
    std::memset(buffer_, 0xAA, sizeof(buffer_));
    for (size_t f = 0; f < kNumFrames; ++f)
      frame_buffers_[f] = buffer_ + kFrameSize * f;
    write_ptr_ = read_ptr_ = 0;
  }

  size_t writeable() const {
    return kNumFrames - readable(); // TODO This might fail in certain cases?
  }

  size_t readable() const {
    return write_ptr_ - read_ptr_;
  }

  // @return readable frame (assumes one exists)
  const uint8_t *readable_frame() const {
    return frame_buffers_[read_ptr_ % kNumFrames];
  }

  // @return next writeable frame (assumes one exists)
  uint8_t *writeable_frame() {
    return frame_buffers_[write_ptr_ % kNumFrames];
  }

  void read() {
    ++read_ptr_;
  }

  void written() {
    ++write_ptr_;
  }

private:

  uint8_t buffer_[kFrameSize * kNumFrames] __attribute__ ((aligned (4)));;
  uint8_t *frame_buffers_[kNumFrames];

  volatile size_t write_ptr_;
  volatile size_t read_ptr_;
};

}; // namespace weegfx

#endif // GFX_FRAMEBUFFER_H_
