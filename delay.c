#include "delay.h"

volatile static uint32_t tick;

void SysTick_Handler(){

	tick++;
	
}
void delay(uint32_t ms){

	tick = 0;
	while(tick < ms);

}

void enable_delay(){

	SysTick_Config(SystemCoreClock / 1000);

}