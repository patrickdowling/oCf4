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

#include "drivers/dac.h"
#include <algorithm>
#include "drivers/gpio.h"
#include "drivers/dac8565.h"

// NOTE With the SPI at 41MHz, we're overclocking the DAC by a good margin.
// From the datasheet p7:
// Maximum SCLK frequency is 50MHz at IOVDD = VDD = 3.6V to 5.5V and 25MHz at IOVDD = AVDD = 2.7V to 3.6V.

// TODO Eval switching SPI to 16 bit mode for transfers?

namespace ocf4 {

void Dac::Init()
{
  gpio.CS_DAC.Set();
  gpio.RST_DAC.Set();

  auto tx_buffer = tx_buffer_.data();
  DAC8565::Pack<DAC8565::WRITE_BUFFER, 0>(0x8000, tx_buffer);
  DAC8565::Pack<DAC8565::WRITE_BUFFER, 1>(0x8000, tx_buffer + 4);
  DAC8565::Pack<DAC8565::WRITE_BUFFER, 2>(0x8000, tx_buffer + 8);
  DAC8565::Pack<DAC8565::WRITE_LOAD_ALL, 3>(0x8000, tx_buffer + 12);
}

void Dac::Update(const std::array<uint16_t, kNumChannels> &values)
{
  auto tx_buffer = tx_buffer_.data();
  auto src = std::begin(values);

  // Interleaved transfers of data with packing of the next.
  // This didn't actually have a meaningful performance impact...
  DAC8565::PackValue(*src++, tx_buffer);
  gpio.CS_DAC.Reset();
  spi_.AsyncTransfer(tx_buffer, 3);
  while (src != std::end(values)) {
    tx_buffer += 4;
    DAC8565::PackValue(*src++, tx_buffer);
    spi_.AsyncTransferWait();
    gpio.CS_DAC.Set();
    gpio.CS_DAC.Reset();
    spi_.AsyncTransfer(tx_buffer, 3);
  }
  spi_.AsyncTransferWait();
  gpio.CS_DAC.Set();

#if 0
  for (size_t i = 0; i < kNumChannels; ++i, tx_buffer += 4) {
    DAC8565::PackValue(*src++, tx_buffer);
    Send(tx_buffer, 3);
  }
#endif
}

}; // namespace ocf4
