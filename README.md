# Stepper-motor-bare-metal-STM32
A non-blocking, bare-metal firmware to control stepper motor with A4988 driver, made on STM32F446RE board using timers and interrupts.

### Compile and Upload
- Update STLink and CMSIS directories in `makefile`.
- If you use VS Code, update these directories in `c_cpp_properties.json`.
- If you are using other STM32 board please provide your own files
- For GDB debugging an appropriate .svd file is provided. If you are using GDB plugin on VS Code, its settings are in `launch.json`.
- Run `make compile upload`