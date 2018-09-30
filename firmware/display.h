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
#ifndef OCF4_DISPLAY_H_
#define OCF4_DISPLAY_H_

#include "drivers/oled_ssd1306.h"
#include "drivers/spi.h"
#include "gfx/framebuffer.h"
#include "gfx/weegfx.h"

namespace ocf4 {

class Display {
public:
  DISALLOW_COPY_AND_ASSIGN(Display);
  Display(Spi &spi)
  : oled_(spi) { }
  ~Display() { }

  void Init();

  void inline Update() __attribute__((always_inline)) {
    if (oled_.frame_valid()) {
      oled_.AsyncWriteNextPage();
    } else if (framebuffer_.readable()) {
      oled_.SetupFrame(framebuffer_.readable_frame());
      oled_.AsyncWriteNextPage();
    }
  }

  void inline Flush() __attribute__((always_inline)) {
    // Check if the async transfer has completed; If it has, the entire frame
    // might have been written also.
    // TODO Better nomenclature, and we can theoretically skip some steps if
    // the transfer is guaranteed to complete by the time we get here.
    if (oled_.frame_valid() && oled_.AsyncWritePageComplete()) {
      if (!oled_.frame_valid())
        framebuffer_.read();
    }
  }

  class Frame {
  public:
    Frame(uint8_t *frame, Display &owner, weegfx::Graphics &gfx) : frame_(frame), owner_(owner), gfx_(gfx) {
      if (valid())
        gfx_.Begin(frame, true);
    }

    ~Frame() {
      if (valid())
        owner_.EndFrame(*this);
    }

    bool valid() const { return frame_; }

    weegfx::Graphics *operator ->() { return &gfx_; }

  private:
    uint8_t *frame_;
    Display &owner_;
    weegfx::Graphics &gfx_;
  };

  Frame BeginFrame() {
    return {
        framebuffer_.writeable() ? framebuffer_.writeable_frame() : nullptr,
        *this,
        gfx_};
  }


  inline weegfx::Graphics &gfx() {
    return gfx_;
  }

private:
  weegfx::Framebuffer<OledSSD1306> framebuffer_;
  weegfx::Graphics gfx_;
  OledSSD1306 oled_;

  void EndFrame(const Frame &frame) {
    if (frame.valid()) {
      gfx_.End();
      framebuffer_.written();
    }
  }
};

extern Display display;
}; // namespace ocf4

#endif // OCF4_DISPLAY_H_