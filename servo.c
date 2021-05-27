#include "servo.h"
#include "GPIO.h"
#include "Timer.h"

void servo_configure(GPIO_Object* gpio, TIM_TypeDef* timer, uint8_t channel){

	gpio_enable_clock(gpio->port);
	gpio_configure_pin_output(gpio, OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL, OUTPUT_SPEED_50MHz);
	
	timer_data data = {.auto_reload_value = 1000, .prescaler = 1440};
	timer_configure(timer, &data);
	timer_pwm_data pwm_data = {.channel = channel, .compare_value = 70};
	timer_configure_pwm(timer, &pwm_data);
	timer_enable(timer);

}
void servo_set_angle(TIM_TypeDef* timer, uint8_t channel, signed int angle){

	if(channel == 1){
		timer->CCR1 = (5 * angle + 630) / 9;
	}


}

