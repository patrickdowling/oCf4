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
// Main runtime for ocf4

#include "drivers/gpio.h"
#include "stm32x/stm32x_core.h"
#include "stm32x/stm32x_debug.h"

#include "display.h"

STM32X_CORE_DEFINE();

namespace ocf4 {
  GPIO gpio;
  Spi spi;
  Display display{spi};

}; // namespace ocf4
using namespace ocf4;
using namespace stm32x;

extern "C" void SysTick_Handler() {
  STM32X_CORE_TICK();

  display.Flush();
  display.Update();
}

int main()
{
  STM32X_DEBUG_INIT(); 
  STM32X_CORE_INIT(F_CPU / 1000);

  display.Init();

  buf[0] = 0xdd;

  while (true) {

    //stm32x::core.Delay(500);

    auto frame = display.BeginFrame();
    if (frame.valid()) {
      frame.graphics().drawFrame(0, 0, 127, 63);
      frame.graphics().drawStr(0, 0, "HELLO");
      frame.graphics().drawStr(0, 8, "WORLD");
    }
  }
}
