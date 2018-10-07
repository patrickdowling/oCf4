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
#include "drivers/ssd1306.h"

namespace ocf4 {

class OledSSD1306 {
public:
  DISALLOW_COPY_AND_ASSIGN(OledSSD1306);
  OledSSD1306(Spi &spi) : spi_(spi) { Init(); }
  ~OledSSD1306() { }

  void InitDisplay(bool display_on);

  static constexpr size_t kDisplayPixelW = WEEGFX_FRAME_W;
  static constexpr size_t kDisplayPixelH = WEEGFX_FRAME_H;

  static constexpr size_t kFrameSize = kDisplayPixelW * kDisplayPixelH / 8;
  static constexpr size_t kNumPages = 8;
  static constexpr size_t kPageSize = kFrameSize / kNumPages;

  static constexpr uint8_t kFadeOutFrames = 32;

  void SetupFrame(const uint8_t *frame);
  bool frame_valid() const {
    return current_frame_;
  }

  enum class AsyncWriteStatus {
    BUSY,
    PAGE_COMPLETE,
    FRAME_COMPLETE,
  };

  void AsyncWriteNextPage();
  AsyncWriteStatus AsyncWritePollStatus();

  void AdjustOffset(uint8_t offset);

  void CmdFadeOut(bool enable); // Not supported on SH1106
  void CmdDisplayOn(bool on);
  void CmdSetContrast(uint8_t contrast);

protected:
  Spi &spi_;

  uint8_t offset_ = 2;
  const uint8_t *current_frame_ = nullptr;
  size_t current_page_ = 0;

  static constexpr size_t kPageHeaderSize = 3;
  uint8_t page_header_[kPageHeaderSize];

  static constexpr size_t kCommandBufferSize = 8;
  uint8_t command_buffer_[kCommandBufferSize];
  volatile size_t command_buffer_length_ = 0;

  void Init();

  inline const uint8_t *current_page_ptr() const {
    return current_frame_ + current_page_ * kPageSize;
  }

  void page_header_init() {
    page_header_[0] = SSD1306::SET_HIGHER_COL_START_ADDRESS /*| (offset_ >> 4)*/; // offset truncated to lower nibble
    page_header_[1] = SSD1306::SET_LOWER_COL_START_ADDRESS | offset_;
    page_header_[2] = SSD1306::SET_PAGE_START_ADDRESS | 0/*page_index*/;
  }

  inline void page_header_set_page(uint8_t page_index) {
    page_header_[2] = SSD1306::SET_PAGE_START_ADDRESS | page_index;
  }

  void command_buffer_reset() {
    command_buffer_length_ = 0;
  }
};

}; // namespace ocf4

#endif // OCF4_DRIVERS_OLEDSSD1306_H_
