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
// DAC8565 driver

#include <algorithm>
#include "drivers/dac.h"
#include "drivers/gpio.h"
#include "drivers/dac8565.h"

namespace ocf4 {

void Dac::Init()
{
  gpio.CS_DAC.Set();
  gpio.RST_DAC.Set();
  std::fill(std::begin(values_), std::end(values_), 0);
}

void Dac::Update()
{
  // TODO Eval switching SPI to 16 bit mode and simplifying
  uint8_t tx_buf[4];
  DAC8565::Pack<DAC8565::WRITE_LOAD, 0>(values_[0], tx_buf);
  Send(tx_buf, 4);
  DAC8565::Pack<DAC8565::WRITE_LOAD, 1>(values_[1], tx_buf);
  Send(tx_buf, 4);
  DAC8565::Pack<DAC8565::WRITE_LOAD, 2>(values_[2], tx_buf);
  Send(tx_buf, 4);
  DAC8565::Pack<DAC8565::WRITE_LOAD, 3>(values_[3], tx_buf);
  Send(tx_buf, 4);
}

}; // namespace ocf4
