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
#include "drivers/ssd1306.h"

namespace ocf4 {

void OledSSD1306::Init()
{
  gpio.CS_OLED.Set();
  gpio.RST.Set();
  gpio.DC.Set();
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
  uint8_t page_write[3];
  page_write[0] = SSD1306::SET_HIGHER_COL_START_ADDRESS /*| (offset_ >> 4)*/; // offset truncated to lower nibble
  page_write[1] = SSD1306::SET_LOWER_COL_START_ADDRESS | offset_;
  page_write[2] = SSD1306::SET_PAGE_START_ADDRESS | current_page_;

  gpio.CS_OLED.Reset();
  gpio.DC.Reset();
  spi_.Send(page_write, sizeof(page_write), true);
  gpio.DC.Set();
  spi_.AsyncTransfer(current_page_ptr(), kPageSize);
}

bool OledSSD1306::AsyncWritePageComplete()
{
  bool complete = spi_.AsyncTransferComplete();
  if (complete) {
    gpio.CS_OLED.Set();
    if (current_page_ >= kNumPages - 1) {
      current_frame_ = nullptr;
      current_page_ = 0;
    } else {
      ++current_page_;
    }
  }
  return complete;
}

void OledSSD1306::DisplayOn(bool on)
{
  gpio.DC.Reset();
  gpio.CS_OLED.Reset();
  spi_.Send8(on ? SSD1306::SET_DISPLAY_ON : SSD1306::SET_DISPLAY_OFF);
  spi_.Flush();
  gpio.CS_OLED.Set();
}

void OledSSD1306::SetContrast(uint8_t contrast)
{
  gpio.DC.Reset();
  gpio.CS_OLED.Reset();
  spi_.Send8(SSD1306::SET_CONTRAST);
  spi_.Send8(contrast);
  spi_.Flush();
  gpio.CS_OLED.Set();
}

}; // namespace ocf4
