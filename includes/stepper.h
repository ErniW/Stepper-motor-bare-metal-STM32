#include "stm32f446xx.h"

enum {
    STEPPER_DIR_COUNTERCLOCKWISE,
    STEPPER_DIR_CLOCKWISE
};

enum {
    STEPPER_STATE_IDLE,
    STEPPER_STATE_BUSY,
    STEPPER_STATE_SLEEPING,
    STEPPER_STATE_STABILIZING
};

class Stepper {
public:
    Stepper(int stepsPerRevolution);
    void timerInit(TIM_TypeDef* timer, int timerChannel, IRQn_Type timerInt, unsigned long APBClockFreq);
    void dirPinInit(GPIO_TypeDef* dirPort, int dirPin);
    void sleepPinInit(GPIO_TypeDef* sleepPort, int sleepPin);
    void start();
    void stop();
    void interruptHandler();
    void nextCommand(long long steps);
    void setDir(bool dir);
    void setSpeed(int speed);
    bool getDir();
    int getSpeed();
    volatile long long getPosition();
    volatile int getState();
    void sleep();
    void wakeup();
private:
    int _stepsPerRevolution;
    int _speed;
    volatile int _state;
    volatile long long _position;
    volatile long long _stepsRemaining;
    volatile int _waitCounter;
    TIM_TypeDef* _timer;
    int _timerChannel;
    IRQn_Type _timerInt;
    GPIO_TypeDef* _dirPort;
    int _dirPin;
    GPIO_TypeDef* _sleepPort;
    int _sleepPin;
};