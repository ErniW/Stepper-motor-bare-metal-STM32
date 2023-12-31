PROJECT_NAME = stepper_motor
BOARD = STM32F446xx
MCU = cortex-m4

STARTUP = STM32F446RE/startup_stm32f446xx.s
SYSTEM = STM32F446RE/system_stm32f4xx.c
LINKERSCRIPT = STM32F446RE/STM32F446RETX_FLASH.ld
CMSIS_INCLUDES_DIR = C:/Users/ernes/STM32Cube/Repository/STM32Cube_FW_F4_V1.26.1/Drivers/CMSIS/Include
STLINK_DIR = stlink/bin

SRCS = main.cpp
SRCS += sources/stepper.cpp
SRCS += sources/queue.cpp
SRCS += $(STARTUP)
SRCS += $(SYSTEM)

INCLUDES = -I ./includes
INCLUDES += -I ./STM32F446RE
INCLUDES += -I $(CMSIS_INCLUDES_DIR)  

CFLAGS = -g -O2 -Wall -T $(LINKERSCRIPT)
CFLAGS += -mlittle-endian -mthumb -mcpu=$(MCU) -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += --specs=nosys.specs

compile:
	arm-none-eabi-g++ $(CFLAGS) $(INCLUDES) $(SRCS) -o ./build/$(PROJECT_NAME).elf -D$(BOARD)
	arm-none-eabi-objcopy -O ihex ./build/$(PROJECT_NAME).elf ./build/$(PROJECT_NAME).hex
	arm-none-eabi-objcopy -O binary ./build/$(PROJECT_NAME).elf ./build/$(PROJECT_NAME).bin


upload:
	$(STLINK_DIR)/st-flash --reset write ./build/$(PROJECT_NAME).bin 0x8000000


clean:
	rm -f ./build/$(PROJECT_NAME).*

