#include <stdbool.h>

#include "stm32f446xx.h"
#include "stepper.h"
#include "queue.h"

#define STEP_PIN            (1 << 5)
#define STEP_PIN_AF_MODE    (1 << 11)
#define STEP_PIN_AF1        (1 << 20)

Stepper motor(400);
Queue commands(10);

int main(){
    RCC->AHB1ENR &=~ RCC_AHB1ENR_GPIOAEN;
     RCC->APB1ENR &=~ RCC_APB1ENR_TIM2EN;
     RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
     RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;


    motor.dirPinInit(GPIOA, 9);
    motor.sleepPinInit(GPIOA, 6);
    motor.timerInit(TIM2, 1, TIM2_IRQn, 16000000);
    motor.setSpeed(150);

   
    
    GPIOA->AFR[0] |= STEP_PIN_AF1;
    GPIOA->MODER |= STEP_PIN_AF_MODE;
    
     //commands.push(-1);
    commands.push(400);
    commands.push(-400);
    commands.push(200);
    commands.push(-200);
    commands.push(400);
    commands.push(-400);
      
    while(true){
        
        switch(motor.getState()){
            case STEPPER_STATE_IDLE:
                if(!commands.isEmpty()){
                    int value = commands.front();
                    commands.pop();
                    motor.nextCommand(value);
                }
                else{
                    motor.sleep();
                }
                break;
            case STEPPER_STATE_SLEEPING:
                if(!commands.isEmpty()){
                    motor.wakeup();
                }
                break;
        }
        
    }
}

extern "C" void TIM2_IRQHandler(void);

void TIM2_IRQHandler(void){
    motor.interruptHandler();
}