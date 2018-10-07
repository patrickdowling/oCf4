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
// SSD1306 driver specifics
#ifndef OCF4_DRIVERS_SSD1306_H_
#define OCF4_DRIVERS_SSD1306_H_

#include <stdint.h>

namespace SSD1306 {
  enum Command : uint8_t {
    SET_LOWER_COL_START_ADDRESS = 0x00,
    SET_HIGHER_COL_START_ADDRESS = 0x10,
    SET_ADDRESSING_MODE = 0x20,
    SET_COLUMN_ADDRESS = 0x21,
    SET_PAGE_ADDRESS = 0x22,
    SET_FADE_OUT = 0x23,
    SCROLL_DEACTIVATE = 0x2E,
    SET_DISPLAY_START_LINE = 0x40,
    SET_CONTRAST = 0x81,
    CHARGE_PUMP_SETTING = 0x8D,
    SET_SEGMENT_REMAP_0SEG0 = 0xA0,
    SET_SEGMENT_REMAP_127SEG0 = 0xA1,
    DISPLAY_OUTPUT_RAM = 0xA4,
    DISPLAY_OUTPUT_FULL = 0xA5,
    SET_DISPLAY_NORMAL = 0xA6,
    SET_DISPLAY_INVERSE = 0xA7,
    SET_MULTIPLEX_RATIO = 0xA8,
    SET_DISPLAY_OFF = 0xAE,
    SET_DISPLAY_ON = 0xAF,
    SET_PAGE_START_ADDRESS = 0xB0,
    SET_COM_OUTPUT_SCAN_0_N = 0xC0,
    SET_COM_OUTPUT_SCAN_N_0 = 0xC8,
    SET_DISPLAY_OFFSET = 0xD3,
    SET_DISPLAY_CLOCK_DIVIDE_RATIO = 0xD5,
    SET_PRECHARGE_PERIOD = 0xD9,
    SET_COM_PIN_CONFIG = 0xDA,
    SET_VCOMH_DESELECT_LEVEL = 0xDB,
  };

  enum FadeMode : uint8_t {
    FADE_DISABLE = 0x00,
    FADE_OUT = 0x20,
    FADE_BLINK = 0x30,
  };

  enum AddressingMode : uint8_t {
    ADDRESSING_MODE_HORIZONTAL = 0x00,
    ADDRESSING_MODE_VERTICAL = 0x01,
    ADDRESSING_MODE_PAGE = 0x02
  };

  static const uint8_t startup_sequence[] = {
    SET_DISPLAY_OFF,
    SET_DISPLAY_CLOCK_DIVIDE_RATIO, 0x80, // [7:4] = set osc frequency
    SET_MULTIPLEX_RATIO, 0x3f, // [5:0]
    SET_DISPLAY_OFFSET, 0x00,
    CHARGE_PUMP_SETTING, 0x14, // 0x10 | [2] = 0b Disable, 1b Enable
    SET_DISPLAY_START_LINE | 0, // 0
    SET_ADDRESSING_MODE, ADDRESSING_MODE_PAGE,
#ifdef FLIP_180
    SET_SEGMENT_REMAP_0SEG0,
    SET_COM_OUTPUT_SCAN_0_N,
#else
    SET_SEGMENT_REMAP_127SEG0,
    SET_COM_OUTPUT_SCAN_N_0,
#endif
    SET_COM_PIN_CONFIG, 0x12,
    SET_CONTRAST, 0xCF,
    SET_PRECHARGE_PERIOD, 0xF1, // A[3:0] = divide ratio, A[7:4] = FOsc
    SET_VCOMH_DESELECT_LEVEL, 0x40, // 0.77 x VCC
    SCROLL_DEACTIVATE,
    DISPLAY_OUTPUT_RAM,
#ifdef INVERT_DISPLAY
    SET_DISPLAY_INVERSE,
#else
    SET_DISPLAY_NORMAL,
#endif
    SET_DISPLAY_ON
  };
};

#endif // OCF4_DRIVERS_SSD1306_H_
