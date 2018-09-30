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
// SSD1306 driver

#ifndef OCF4_DRIVERS_OLEDSSD1306_H_
#define OCF4_DRIVERS_OLEDSSD1306_H_

#include "util/util_macros.h"
#include "drivers/spi.h"

namespace ocf4 {

class OledSSD1306 {
public:
  DISALLOW_COPY_AND_ASSIGN(OledSSD1306);
  OledSSD1306(Spi &spi) : spi_(spi) { Init(); }
  ~OledSSD1306() { }

  void Init();
  void InitDisplay(bool display_on);

  static constexpr size_t kDisplayPixelW = WEEGFX_FRAME_W;
  static constexpr size_t kDisplayPixelH = WEEGFX_FRAME_H;

  static constexpr size_t kFrameSize = kDisplayPixelW * kDisplayPixelH / 8;
  static constexpr size_t kNumPages = 8;
  static constexpr size_t kPageSize = kFrameSize / kNumPages;

  void SetupFrame(const uint8_t *frame);
  bool frame_valid() const {
    return current_frame_;
  }

  void AsyncWriteNextPage();
  bool AsyncWritePageComplete();

  void DisplayOn(bool on);
  void SetContrast(uint8_t contrast);
  void AdjustOffset(uint8_t offset) {
    offset_ = offset & 0x0f;
  }

protected:
  Spi &spi_;

  uint8_t offset_ = 2;
  const uint8_t *current_frame_ = nullptr;
  size_t current_page_ = 0;

  inline const uint8_t *current_page_ptr() const {
    return current_frame_ + current_page_ * kPageSize;
  }
};

}; // namespace ocf4

#endif // OCF4_DRIVERS_OLEDSSD1306_H_
