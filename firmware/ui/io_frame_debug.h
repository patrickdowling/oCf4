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
#ifndef OCF4_IO_FRAME_DEBUG_H_
#define OCF4_IO_FRAME_DEBUG_H_

#include "ui/debug_menu.h"
#include "io_frame.h"

namespace ocf4 {

class IOFrameDebug : public Debuggable {
public:
  DISALLOW_COPY_AND_ASSIGN(IOFrameDebug);
  IOFrameDebug(const IOFrame &io_frame) : io_frame_(io_frame) { }

  virtual void DebugView(Display::Frame &frame) const final {
    weegfx::coord_t y = 16;
    std::for_each(
        std::begin(io_frame_.adc_in),
        std::end(io_frame_.adc_in),
        [&](int32_t value) {
          frame->printf(0, y, "%6ld", value);
          frame->drawRect(60, y + 2, (value & 15) * 4, 4);
          y += 8;
        }
      );
    frame->drawHLine(60, y + 2, 64);
    for (weegfx::coord_t x = 0; x <= 16; ++x)
      frame->setPixel(60 + x * 4, y + 1);

  }

private:
  const IOFrame &io_frame_;
};

}; // namespace ocf4

#endif // OCF4_IO_FRAME_DEBUG_H_
