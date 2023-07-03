#include "stepper.h"
#include <math.h>
#include <stdlib.h>

Stepper::Stepper(int stepsPerRevolution){
    _stepsPerRevolution = stepsPerRevolution;
    _state = STEPPER_STATE_IDLE;
}

void Stepper::dirPinInit(GPIO_TypeDef* dirPort, int dirPin){
    _dirPort = dirPort;
    _dirPin = dirPin;
    _dirPort->MODER |= 1 << _dirPin * 2;
}

void Stepper::sleepPinInit(GPIO_TypeDef* sleepPort, int sleepPin){
    _sleepPort = sleepPort;
    _sleepPin = sleepPin;
    _sleepPort->MODER |= 1 << _dirPin * 2;
}

void Stepper::timerInit(TIM_TypeDef* timer, int timerChannel, IRQn_Type timerInt, unsigned long APBClockFreq){
    _timer = timer;
    _timerChannel = timerChannel;
    _timerInt = timerInt;

    _timer->CR1 &=~ TIM_CR1_CEN;
    _timer->PSC = (APBClockFreq / 1000000) - 1;
    _timer->ARR = 5000;
    _timer->CCR1 = PULSE_LENGTH;

    if(_timerChannel == 1)      _timer->CCMR1 |= TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2;
    else if(_timerChannel == 2) _timer->CCMR1 |= TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2;
    else if(_timerChannel == 3) _timer->CCMR2 |= TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2;
    else if(_timerChannel == 4) _timer->CCMR2 |= TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2;
    
    _timer->DIER |= 1 << _timerChannel;
    _timer->CR1 |= TIM_CR1_CEN;

    NVIC_EnableIRQ(_timerInt);
}

void Stepper::start(){
    _state = STEPPER_STATE_BUSY;
    _timer->CCER |= 1 << (_timerChannel - 1) * 4; 
}

void Stepper::stop(){
    _state = STEPPER_STATE_IDLE;
    _timer->CCER &=~ 1 << (_timerChannel - 1) * 4;
}

void Stepper::interruptHandler(){

    if(!(_timer->SR & TIM_SR_CC1IF)) return;

    _timer->SR &=~ TIM_SR_CC1IF; 

    switch(getState()){
        case STEPPER_STATE_BUSY:
            
            if(_stepsRemaining > 0){
                _stepsRemaining--;

                if(getDir() == STEPPER_DIR_CLOCKWISE) _position++;
                else _position--;
            }
            else {
                stop();
            }
            
        break;

        case STEPPER_STATE_STABILIZING:
            
            if(_waitCounter > 0) _waitCounter--;
            else{
                _state = STEPPER_STATE_BUSY;

                if(_timerChannel == 1)      _timer->CCMR1 |= TIM_CCMR1_OC1M_2;
                else if(_timerChannel == 2) _timer->CCMR1 |= TIM_CCMR1_OC2M_2;
                else if(_timerChannel == 3) _timer->CCMR2 |= TIM_CCMR2_OC3M_2;
                else if(_timerChannel == 4) _timer->CCMR2 |= TIM_CCMR2_OC4M_2;
            }

        break;
    } 
}

void Stepper::nextCommand(long long steps){

    bool dirChanged;

    if(steps > 0) dirChanged = setDir(STEPPER_DIR_CLOCKWISE);
    else dirChanged = setDir(STEPPER_DIR_COUNTERCLOCKWISE);

    _stepsRemaining = abs(steps);

    if(!dirChanged) start();
    else _timer->CCER |= 1 << (_timerChannel - 1) * 4; 
}

bool Stepper::setDir(bool dir){

    if(getDir() == dir) return false;

    _state = STEPPER_STATE_STABILIZING;
    _waitCounter = WAIT_TIME;

    if(_timerChannel == 1)      _timer->CCMR1 &=~ TIM_CCMR1_OC1M_2;
    else if(_timerChannel == 2) _timer->CCMR1 &=~ TIM_CCMR1_OC2M_2;
    else if(_timerChannel == 3) _timer->CCMR2 &=~ TIM_CCMR2_OC3M_2;
    else if(_timerChannel == 4) _timer->CCMR2 &=~ TIM_CCMR2_OC4M_2;

    _dirPort->ODR ^= 1 << _dirPin;
    return true;
}

bool Stepper::getDir(){
    bool state = _dirPort->ODR & (1 << _dirPin);
    return state;
}

void Stepper::setSpeed(int rpm){

    _timer->CR1 &=~ TIM_CR1_CEN;

    float freq = (_stepsPerRevolution * rpm) / 60;
    unsigned int arr = 1000000.0 / freq;

    _timer->ARR = arr - 1;
    _speed = rpm;

    _timer->CR1 |= TIM_CR1_CEN;
}

int Stepper::getSpeed(){
    return _speed;
}

volatile int Stepper::getState(){
    return _state;
}

volatile long long Stepper::getPosition(){
    return _position;
}

void Stepper::sleep(){
    stop();
    _state = STEPPER_STATE_SLEEPING;
    _sleepPort->ODR &=~ 1 << _sleepPin;
}

void Stepper::wakeup(){
    _state = STEPPER_STATE_IDLE;
    _sleepPort->ODR |= 1 << _sleepPin;
}