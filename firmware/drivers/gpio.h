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
//
// AUTOMATICALLY GENERATED FILE, DO NOT EDIT
//
// IOC source: ../stm32/STM32F405RGTx.ioc
// CSV source: ../stm32/STM32F405RGTx.csv
// --------------------------------------------------------------------------------
#ifndef OCF4_GPIO_H_
#define OCF4_GPIO_H_
#include "stm32x/stm32x_core.h"

namespace ocf4 {
class GPIO {
  struct GPIO_PORT_INIT {
    GPIO_PORT_INIT() {
      stm32x::GPIOx<stm32x::GPIO_PORT_A>::EnableClock(true);
      stm32x::GPIOx<stm32x::GPIO_PORT_C>::EnableClock(true);
      stm32x::GPIOx<stm32x::GPIO_PORT_B>::EnableClock(true);
      stm32x::GPIOx<stm32x::GPIO_PORT_D>::EnableClock(true);
    }
  };
  GPIO_PORT_INIT port_initializer_;

public:
  DISALLOW_COPY_AND_ASSIGN(GPIO);
  GPIO() { };
  ~GPIO() { };
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<0, stm32x::GPIO_PUPD::PULLUP> ENC_R2;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<1, stm32x::GPIO_PUPD::PULLUP> ENC_R1;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<2, stm32x::GPIO_PUPD::PULLUP> SW_R;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<3, stm32x::GPIO_PUPD::NONE> GND;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_OUT<4, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> DBG;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AF<5, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI1*/> SPI1_SCK;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<6, stm32x::GPIO_PUPD::PULLUP> ENC_L2;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AN<7> CV2;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_OUT<8, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> USB_PWR_EN;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<9, stm32x::GPIO_PUPD::NONE> USB_OTG_FS_VBUS;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AF<10, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 1/*GPIO_AF10_OTG_FS*/> USB_OTG_FS_ID;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AF<11, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 1/*GPIO_AF10_OTG_FS*/> USB_OTG_FS_DM;
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_AF<12, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 1/*GPIO_AF10_OTG_FS*/> USB_OTG_FS_DP;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_A>::GPIO_IN<15, stm32x::GPIO_PUPD::PULLUP> SW_T;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AN<0> CV1;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_IN<1, stm32x::GPIO_PUPD::NONE> USB_PWR_FAULT;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_IN<2, stm32x::GPIO_PUPD::PULLUP> ENC_L1;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<3, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> PB3;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<4, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> PB4;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<5, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI1*/> SPI1_MOSI;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<6, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> RST;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_OUT<7, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> CS_OLED;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<8, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 4/*GPIO_AF4_I2C1*/> I2C1_SCL;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<9, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 4/*GPIO_AF4_I2C1*/> I2C1_SDA;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<10, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 7/*GPIO_AF7_USART3*/> USART3_TX;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<11, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 7/*GPIO_AF7_USART3*/> USART3_RX;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_IN<12, stm32x::GPIO_PUPD::PULLUP> SW_L;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<13, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI2*/> SPI2_SCK;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<14, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI2*/> SPI2_MISO;
  stm32x::GPIOx<stm32x::GPIO_PORT_B>::GPIO_AF<15, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE, 5/*GPIO_AF5_SPI2*/> SPI2_MOSI;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_OUT<0, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> FLASH_CS;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_AN<4> CV4;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_AN<5> CV3;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_AF<6, stm32x::GPIO_SPEED::LOW, stm32x::GPIO_OTYPE::OD, stm32x::GPIO_PUPD::PULLUP, 2/*GPIO_AF2_TIM3*/> TR2;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_AF<7, stm32x::GPIO_SPEED::LOW, stm32x::GPIO_OTYPE::OD, stm32x::GPIO_PUPD::PULLUP, 2/*GPIO_AF2_TIM3*/> TR1;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_AF<8, stm32x::GPIO_SPEED::LOW, stm32x::GPIO_OTYPE::OD, stm32x::GPIO_PUPD::PULLUP, 2/*GPIO_AF2_TIM3*/> TR3;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_AF<9, stm32x::GPIO_SPEED::LOW, stm32x::GPIO_OTYPE::OD, stm32x::GPIO_PUPD::PULLUP, 2/*GPIO_AF2_TIM3*/> TR4;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_IN<10, stm32x::GPIO_PUPD::PULLUP> SW_B;
#if 0
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_OUT<11, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> PC11;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_OUT<12, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> PC12;
#endif
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_OUT<13, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> RST_DAC;
  stm32x::GPIOx<stm32x::GPIO_PORT_C>::GPIO_OUT<12, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> CS_DAC;
  stm32x::GPIOx<stm32x::GPIO_PORT_D>::GPIO_OUT<2, stm32x::GPIO_SPEED::MEDIUM, stm32x::GPIO_OTYPE::PP, stm32x::GPIO_PUPD::NONE> DC;
}; // class GPIO

extern GPIO gpio;
}; // namespace ocf4
#endif // OCF4_GPIO_H_
