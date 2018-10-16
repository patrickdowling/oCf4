# Copyright 2018 Patrick Dowling.
#
# Author: Patrick Dowling (pld@gurkenkiste.com)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
# See http://creativecommons.org/licenses/MIT/ for more information.

TOOLCHAIN_PATH ?= ~/dev/gcc-arm-none-eabi-7-2017-q4-major/

###
## Project config
#
PROJECT = ocf4
PROJECT_INCLUDE_DIRS = .
PROJECT_SRC_DIRS = . drivers ui gfx builtin
PROJECT_RESOURCE_DIR = resources/
PROJECT_RESOURCE_SCRIPT = $(PROJECT_RESOURCE_DIR)resources.py

#PROJECT_DEFINES += USE_FULL_ASSERT
PROJECT_DEFINES += WEEGFX_FRAME_W=128 WEEGFX_FRAME_H=64
PROJECT_DEFINES += OCF4_ENABLE_PROFILE

# NOTE: This requires a bunch of flash
PROJECT_LINKER_FLAGS += -u _printf_float

RAM_SIZE     = 128K
CCM_RAM_SIZE = 64K
FLASH_SIZE   = 1024K
MIN_STACK_SIZE = 2K
#FLASH_SETTINGS_SIZE = 1K
ENABLE_LIBC_INIT_ARRAY = TRUE
#ENABLE_CCM_STACK = TRUE -> Need to fix _sbrk, which is triggered by fprintf("%f")

MODEL = STM32F40_41xxx
F_CPU = 168000000L
HSE_VALUE = 8000000L

include stm32x/makefile

# Experimental
IOC_FILE = ../stm32/STM32F405RGTx.ioc
CSV_FILE = ../stm32/STM32F405RGTx.csv
CUBEMX_ROOT = /Applications/STMicroelectronics/STM32CubeMX.app/Contents/
CUBEMX ?= "$(CUBEMX_ROOT)Resources"

PINOUT_SCRIPT = $(BUILD_DIR)cubemx.scr

$(PINOUT_SCRIPT): $(BUILD_DIR) $(IOC_FILE)
	$(ECHO) "Building CubeMX script $@"
	@echo "config load $(IOC_FILE)" > $@
	@echo "csv pinout $(CSV_FILE)" >> $@
	@echo "exit" >> $@

pinout: $(PINOUT_SCRIPT)
	java -jar "$(CUBEMX_ROOT)MacOs/STM32CubeMX" -s $(PINOUT_SCRIPT)
	python $(STM32X_DIR)tools/stm32x_cubemx_gpio_export.py $(IOC_FILE) $(CSV_FILE) \
		--namespace $(PROJECT) --numeric \
		--cubemx $(CUBEMX) \
		-o ./drivers/gpio

FLASH_TARGET = flash_openocd
OPENOCD_INTERFACE = ftdi/olimex-arm-usb-tiny-h
OPENOCD = openocd -f interface/$(OPENOCD_INTERFACE).cfg -f interface/ftdi/olimex-arm-jtag-swd.cfg -f target/stm32f4x.cfg

.PHONY: gdb_server
gdb_server: $(ELFFILE)
	$(OPENOCD) \
		-c "init" -c "reset" -c "halt"

.PHONY: gdb_client
gdb_client: $(ELFFILE)
	$(Q)$(GDB) --ex 'target remote localhost:3333' $(ELFFILE)

.PHONY: flash_openocd
flash_openocd: $(BINFILE)
	$(OPENOCD) \
		-c "init" -c "halt" -c "sleep 1000" \
		-c "flash write_image erase $(BINFILE) 0x08000000" \
		-c "sleep 200" -c "reset run" -c "shutdown"
