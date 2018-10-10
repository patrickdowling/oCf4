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
// DAC8565 specifics

#ifndef OCF4_DRIVERS_DAC8565_H_
#define OCF4_DRIVERS_DAC8565_H_

#include <stdint.h>

namespace DAC8565 {

enum Command : uint8_t {
  WRITE_BUFFER = 0x00,
  WRITE_LOAD = 0x10,
  WRITE_LOAD_ALL = 0x20
};

template<Command command, uint8_t channel>
inline void Pack(uint16_t value, uint8_t *tx_buffer) {
  tx_buffer[0] = command | (channel << 1);
  tx_buffer[1] = value >> 8;
  tx_buffer[2] = value;
  tx_buffer[3] = 0;
}

inline void PackValue(uint16_t value, uint8_t *tx_buffer) {
  tx_buffer[1] = value >> 8;
  tx_buffer[2] = value;
}


}; // namespace DAC8565

#endif // OCF4_DRIVERS_DAC8565_H_
