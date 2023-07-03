# Stepper-motor-bare-metal-STM32
A non-blocking, bare-metal firmware to control stepper motor with A4988 driver, made on STM32F446RE board using timers and interrupts. Code uses queue data structure to sequence the movement command but its usage is not mandatory.

### Circuit
Connect the A4988 driver:
- **Step pin:** `PA5` which is `D13`. Thanks to build-in LED you can see it's running without plugging the motor. This example uses **Timer 2 channel 1**. If you want to use other timer, remember to set the corrsponding pin alternate function (the library won't switch it manually).
- **Dir pin:** `PA9` which is `D8`. 
- **Sleep pin:** `PA8` which is `D7`. Optional.

Remember to correctly set the current limit on the driver!

### Compile and Upload
- Update STLink and CMSIS directories in `makefile`.
- If you use VS Code, update these directories in `c_cpp_properties.json`.
- If you are using other STM32 board please provide your own files.
- For GDB debugging an appropriate .svd file is provided. If you are using GDB plugin on VS Code, its settings are in `launch.json`.
- Run `make compile upload`

### Library
**Initialize the stepper.**

    Stepper(int stepsPerRevolution);

**Initialize the stepper timer.**

    void timerInit(TIM_TypeDef* timer, int timerChannel, IRQn_Type timerInt, unsigned long APBClockFreq);

- **Timer** is the target timer and **timerChannel** is the stepper motor channel.
- **timerInt** is the timer interrupt.
- **APBClockFreq** you have to pass the corresponding clock speed depending on chosen timer.

Important: alternate function of step pin must be set manually to corresponding timer.

**Set the dir pin**

    void setDirPin(GPIO_TypeDef* dirPort, int dirPin);

Set the port and pin number.

**Set the sleep pin**

    void setSleepPin(GPIO_TypeDef* sleepPort, int sleepPin);

Set the port and pin number.

**Set the speed**

    void setSpeed(int speed);

Set speed in RPM. Be aware that speed over 150 can cause stepper to block. You can play with `WAIT_TIME` to see how it works.

**Trigger next command and move by X steps**

    void nextCommand(long long steps);

The sign determines the direction. If command has the same as previous direction it will move, when it has to change the direction it will go to stabilize state before moving.

**For other utility methods please refer to `stepper.h` file.**

### States
The motor can be in 4 states:
- **IDLE:** Not moving.
- **BUSY:** Moving. The `nextCommand` sets the movement.
- **SLEEPING:** Sleeping. On wakeup it goes to idle state.
- **STABILIZING** The PWM signal is forced to be low for some cycles. It's necessary to give a short break when changing direction, otherwise the motor shaft can block. By doing it like that the stepper motor movement is truly timer-only, and eventually non-blocking.
