-include ../config.mk

OPENOCD ?= sudo openocd
STM32CUBE ?= ../cube

SRC = main bitbang_i2c debounce rotary stm32f0xx_it

LIBSRC  = stm32f0xx_hal_cortex.c stm32f0xx_hal_tim_ex.c stm32f0xx_hal_tim.c
LIBSRC += stm32f0xx_hal_gpio.c stm32f0xx_hal.c stm32f0xx_hal_rcc.c
LIBSRC += stm32f0xx_hal_adc.c stm32f0xx_hal_i2c.c stm32f0xx_hal_i2c_ex.c

TEMPLATE_SRC = system_stm32f0xx.c

STARTUP = $(STM32CUBE)/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/gcc/startup_stm32f030xc.s

DEFS  = -DSTM32F030x6 -DUSE_HAL_DRIVER
DEFS += -DHAL_ADC_MODULE_ENABLED
DEFS += -DHAL_I2C_MODULE_ENABLED
DEFS += -DHAL_TIM_MODULE_ENABLED

INCS  = -I .
INCS += -I $(STM32CUBE)/Drivers/STM32F0xx_HAL_Driver/Inc
INCS += -I $(STM32CUBE)/Drivers/CMSIS/Device/ST/STM32F0xx/Include
INCS += -I $(STM32CUBE)/Drivers/CMSIS/Include
INCS += -I $(STM32CUBE)/Drivers/BSP/STM32F0xx-Nucleo_32
INCS += -I $(STM32CUBE)/Drivers/BSP/STM32F0xx_Nucleo_32

CFLAGS  = -mcpu=cortex-m0 -mlittle-endian -mthumb
CFLAGS += -ffunction-sections -fdata-sections
CFLAGS +=  $(INCS) $(DEFS)

LDFLAGS  = -Wl,--gc-sections -T030.ld --specs=nano.specs --specs=rdimon.specs
LDFLAGS += -lc -lrdimon

unused := $(shell mkdir -p ../bin)

all: ../bin/main.elf

install: ../bin/main.elf
	$(OPENOCD) --file openocd.cfg --command "program $< verify reset exit"

install-server: ../bin/main.bin
	echo "program $< verify reset" | socat -u - tcp:localhost:4444

../bin/main.elf: $(patsubst %,../bin/%.o,$(SRC)) $(patsubst %.c,../bin/%.o,$(LIBSRC) $(TEMPLATE_SRC)) $(STARTUP)
	arm-none-eabi-gcc -Wall -g -Os -o $@ $^ $(CFLAGS) $(LDFLAGS)

../bin/%.o: %.cc
	arm-none-eabi-gcc -Wall -std=c++11 -g -Os -c -o $@ $< $(CFLAGS)

../bin/%.o: %.c
	arm-none-eabi-gcc -Wall -g -Os -c -o $@ $< $(CFLAGS)

../bin/%.o: $(STM32CUBE)/Drivers/CMSIS/Device/ST/STM32F0xx/Source/Templates/%.c
	arm-none-eabi-gcc -Wall -g -Os -c -o $@ $< -std=c11 $(CFLAGS)

../bin/%.o: $(STM32CUBE)/Drivers/STM32F0xx_HAL_Driver/Src/%.c
	arm-none-eabi-gcc -Wall -g -Os -c -o $@ $< -std=c11 $(CFLAGS)
