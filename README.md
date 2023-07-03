# Stepper-motor-bare-metal-STM32
A non-blocking, bare-metal firmware to control stepper motor with A4988 driver, made on STM32F446RE board using timers and interrupts. Code uses queue data structure to sequence the movement command but its usage is not mandatory.

### Circuit
Connect the A4988 driver:
- **Step pin:** `PA5` which is `D13`. Thanks to build-in LED you can see it's running without plugging the motor. This example uses **Timer 2 channel 1**. If you want to use other timer, remember to set the corrsponding pin alternate function (the library won't switch it manually).
- **Dir pin:** `PA9` which is `D8`. 
- **Sleep pin:** `PA8` which is `D7`. Optional.

Remember to correctly set the current limit on the driver!

### States
The motor can be in 4 states:
- **IDLE:** Not moving.
- **BUSY:** Moving. The `nextCommand` sets the movement.
- **SLEEPING:** Sleeping. On wakeup it goes to idle state.
- **STABILIZING** The PWM signal is forced to be low for some cycles. It's necessary to give a short break when changing direction, otherwise the motor shaft can block. By doing it like that the stepper motor movement is truly timer-only, and eventually non-blocking.

### Compile and Upload
- Update STLink and CMSIS directories in `makefile`.
- If you use VS Code, update these directories in `c_cpp_properties.json`.
- If you are using other STM32 board please provide your own files.
- For GDB debugging an appropriate .svd file is provided. If you are using GDB plugin on VS Code, its settings are in `launch.json`.
- Run `make compile upload`