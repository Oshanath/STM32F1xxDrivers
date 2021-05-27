#ifndef Servo
#define Servo
#include "stm32f10x.h"
#include "GPIO.h"

void servo_configure(GPIO_Object* gpio, TIM_TypeDef* timer, uint8_t channel);
void servo_set_angle(TIM_TypeDef* timer, uint8_t channel, signed int angle);

#endif