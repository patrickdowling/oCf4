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
// SSD1306 driver (over SPI)

#include "drivers/oled_ssd1306.h"
#include <cstring>
#include "drivers/gpio.h"

namespace ocf4 {

void OledSSD1306::Init()
{
  gpio.CS_OLED.Set();
  gpio.RST.Set();
  gpio.DC.Set();

  page_header_init();
  command_buffer_reset();
}

void OledSSD1306::InitDisplay(bool display_on)
{
  // Reset
  gpio.RST.Reset();
  gpio.CS_OLED.Reset();

  gpio.RST.Set();
  stm32x::core.Delay(2);
  gpio.RST.Reset();
  stm32x::core.Delay(10);
  gpio.RST.Set();

  gpio.CS_OLED.Set();
  gpio.DC.Reset();
  gpio.RST.Reset();
  stm32x::core.Delay(20);
  gpio.RST.Set();
  stm32x::core.Delay(20);

  // Startup sequence
  gpio.DC.Reset();
  gpio.CS_OLED.Reset();

  size_t sequence_length = sizeof(SSD1306::startup_sequence);
  if (!display_on)
    --sequence_length;
  spi_.Send(SSD1306::startup_sequence, sequence_length, true);
  gpio.CS_OLED.Set();
}

void OledSSD1306::SetupFrame(const uint8_t *frame)
{
  // TODO inject commands here, use member array for page_write header

  current_frame_ = frame;
  current_page_ = 0;
}

void OledSSD1306::AsyncWriteNextPage()
{
  page_header_set_page(current_page_);

  gpio.CS_OLED.Reset();
  gpio.DC.Reset();
  if (command_buffer_length_) {
    spi_.Send(command_buffer_, command_buffer_length_, false);
    command_buffer_reset();
  }
  spi_.Send(page_header_, kPageHeaderSize, true);
  gpio.DC.Set();
  spi_.AsyncTransfer(current_page_ptr(), kPageSize);
}

OledSSD1306::AsyncWriteStatus OledSSD1306::AsyncWritePollStatus()
{
  if (!spi_.AsyncTransferComplete())
    return AsyncWriteStatus::BUSY;

  gpio.CS_OLED.Set();
  if (current_page_ >= kNumPages - 1) {
    current_frame_ = nullptr;
    current_page_ = 0;
    return AsyncWriteStatus::FRAME_COMPLETE;
  } else {
    ++current_page_;
    return AsyncWriteStatus::PAGE_COMPLETE;
  }
}

void OledSSD1306::AdjustOffset(uint8_t offset)
{
  offset_ = offset & 0x0f;
  page_header_[1] = SSD1306::SET_LOWER_COL_START_ADDRESS | offset_;
}

void OledSSD1306::CmdFadeOut(bool enable)
{
  command_buffer_[0] = SSD1306::SET_FADE_OUT;
  if (enable)
    command_buffer_[1] = SSD1306::FADE_OUT | kFadeOutFrames;
  else
    command_buffer_[1] = SSD1306::FADE_DISABLE;
  command_buffer_length_ = 2;
}


void OledSSD1306::CmdDisplayOn(bool on)
{
  command_buffer_[0] = on ? SSD1306::SET_DISPLAY_ON : SSD1306::SET_DISPLAY_OFF;
  command_buffer_length_ = 1;
}

void OledSSD1306::CmdSetContrast(uint8_t contrast)
{
  command_buffer_[0] = SSD1306::SET_CONTRAST;
  command_buffer_[1] = contrast;
  command_buffer_length_ = 2;
}

}; // namespace ocf4
