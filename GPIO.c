#include "GPIO.h"

static void (*EXTI0_Handle)(void);
static void (*EXTI1_Handle)(void);
static void (*EXTI2_Handle)(void);
static void (*EXTI3_Handle)(void);
static void (*EXTI4_Handle)(void);

void gpio_enable_clock(GPIO_TypeDef* port){

	uint32_t val;
	
	if(port == GPIOA){
		val = RCC_APB2ENR_IOPAEN;
	}
	else if(port == GPIOB){
		val = RCC_APB2ENR_IOPBEN;
	}
	else if(port == GPIOC){
		val = RCC_APB2ENR_IOPCEN;
	}
	else if(port == GPIOD){
		val = RCC_APB2ENR_IOPDEN;
	}
	else if(port == GPIOE){
		val = RCC_APB2ENR_IOPEEN;
	}
	else if(port == GPIOF){
		val = RCC_APB2ENR_IOPFEN;
	}
	else{
		val = RCC_APB2ENR_IOPGEN;
	}
	
	RCC->APB2ENR |= val;
	
}

void gpio_configure_pin_output(GPIO_Object* gpio, uint8_t mode, uint8_t speed){

	gpio_enable_clock(gpio->port);

	volatile uint32_t* correct_config_register;
	uint32_t correct_pin;
	
	if(gpio->pin > 7){
		correct_config_register = &gpio->port->CRH;
		correct_pin = gpio->pin - 8;
	}
	else{
		correct_config_register = &gpio->port->CRL;
		correct_pin = gpio->pin;
	}
	
	uint32_t port_start = correct_pin * 4;
	
	if(mode == OUTPUT_GENERAL_PURPOSE_PUSH_PULL){
		*correct_config_register &= ~(1 << (port_start + CNF1));
		*correct_config_register &= ~(1 << (port_start + CNF0));
	}
	else if(mode == OUTPUT_GENERAL_PURPOSE_OPEN_DRAIN){
		*correct_config_register &= ~(1 << (port_start + CNF1));
		*correct_config_register |= (1 << (port_start + CNF0));
	}
	else if(mode == OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL){
		*correct_config_register |= (1 << (port_start + CNF1));
		*correct_config_register &= ~(1 << (port_start + CNF0));
	}
	else if(mode == OUTPUT_ALTERNATE_FUNCTION_OPEN_DRAIN){
		*correct_config_register |= (1 << (port_start + CNF1));
		*correct_config_register |= (1 << (port_start + CNF0));
	}
	
	
	
	if(speed == OUTPUT_SPEED_10MHz){
		*correct_config_register &= ~(1 << (port_start + MODE1));
		*correct_config_register |= (1 << (port_start + MODE0));
	}
	else if(speed == OUTPUT_SPEED_2MHz){
		*correct_config_register |= (1 << (port_start + MODE1));
		*correct_config_register &= ~(1 << (port_start + MODE0));
	}
	else if(speed == OUTPUT_SPEED_50MHz){
		*correct_config_register |= (1 << (port_start + MODE1));
		*correct_config_register |= (1 << (port_start + MODE0));
	}
	
}

void gpio_write_pin(GPIO_Object* gpio, uint8_t state){
	
	if(state){
		gpio->port->BSRR = 1 << gpio->pin;
	}
	else{
		gpio->port->BSRR = 1 << (gpio->pin + 16);
	}
	
}

void gpio_toggle_pin(GPIO_Object* gpio){

	gpio->port->ODR ^= 1 << gpio->pin;

}
void gpio_configure_pin_input(GPIO_Object* gpio, uint8_t mode){

	gpio_enable_clock(gpio->port);

	volatile uint32_t* correct_config_register;
	uint32_t correct_pin;
	
	if(gpio->pin > 7){
		correct_config_register = &(gpio->port)->CRH;
		correct_pin = gpio->pin - 8;
	}
	else{
		correct_config_register = &gpio->port->CRL;
		correct_pin = gpio->pin;
	}
	
	uint32_t port_start = correct_pin * 4;
	
	if(mode == INPUT_ANALOG){
		*correct_config_register &= ~(1 << (port_start + CNF1));
		*correct_config_register &= ~(1 << (port_start + CNF0));
	}
	else if(mode == INPUT_FLOATING){
		*correct_config_register &= ~(1 << (port_start + CNF1));
		*correct_config_register |= (1 << (port_start + CNF0));
	}
	else if(mode == INPUT_PULLUP || mode == INPUT_PULLDOWN){
		*correct_config_register |= (1 << (port_start + CNF1));
		*correct_config_register &= ~(1 << (port_start + CNF0));
		
		if(mode == INPUT_PULLUP){
			gpio_write_pin(gpio, 1);
		}
		else{
			gpio_write_pin(gpio, 0);
		}
	}
	
	*correct_config_register &= ~(1 << (port_start + MODE1));
	*correct_config_register &= ~(1 << (port_start + MODE0));
	
}

uint32_t gpio_read_pin(GPIO_Object* gpio){

	return ((gpio->port->IDR & (1 << gpio->pin)) >> gpio->pin);
	
}

uint32_t gpio_read_port(GPIO_TypeDef* port){

	return (port->IDR);

}

void gpio_configure_interrupt(GPIO_Object* gpio, uint32_t edge, void(*handle)(void)){

	gpio_enable_clock(gpio->port);
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	uint32_t port_value;
	
	if(gpio->port == GPIOA){
		port_value = PORTA_VALUE;
	}
	else if(gpio->port == GPIOB){
		port_value = PORTB_VALUE;
	}
	else if(gpio->port == GPIOC){
		port_value = PORTC_VALUE;
	}
	else if(gpio->port == GPIOD){
		port_value = PORTD_VALUE;
	}
	else if(gpio->port == GPIOE){
		port_value = PORTE_VALUE;
	}
	else if(gpio->port == GPIOF){
		port_value = PORTF_VALUE;
	}
	else {
		port_value = PORTG_VALUE;
	}
	
	AFIO->EXTICR[gpio->pin/4] |= (port_value << ((gpio->pin%4)*4));
	
	if(edge == RISING_AND_FALLING_EDGES || edge == RISING_EDGE){
		EXTI->RTSR |= 1 << gpio->pin;
	}
	
	if(edge == RISING_AND_FALLING_EDGES || edge == FALLING_EDGE){
		EXTI->FTSR |= 1 << gpio->pin;
	}

	
	uint32_t irqn;
	
	if(gpio->pin == 0){
		irqn = EXTI0_IRQn;
		EXTI0_Handle = handle;
		
	}
	else if(gpio->pin == 1){
		irqn = EXTI1_IRQn;
		EXTI1_Handle = handle;
	}
	else if(gpio->pin == 2){
		irqn = EXTI2_IRQn;
		EXTI2_Handle = handle;
	}
	else if(gpio->pin == 3){
		irqn = EXTI3_IRQn;
		EXTI3_Handle = handle;
	}
	else if(gpio->pin == 4){
		irqn = EXTI4_IRQn;
		EXTI4_Handle = handle;
	}
	else if(gpio->pin <= 9 && gpio->pin >= 5){
		irqn = EXTI9_5_IRQn;
	}
	else {
		irqn = EXTI15_10_IRQn;
	}
	
	NVIC_EnableIRQ(irqn);
}

void gpio_enable_interrupt(uint32_t pin){

	EXTI->IMR |= 1 << pin;

}

void gpio_clear_interrupt(uint32_t pin){

	EXTI->PR |= 1 << pin;

}

void gpio_generate_software_interrupt(uint32_t pin){

	EXTI->SWIER |= (1 << pin);
	
}


void EXTI0_IRQHandler(void){
	gpio_clear_interrupt(0);
	EXTI0_Handle();
}

void EXTI1_IRQHandler(void){
	gpio_clear_interrupt(1);
	EXTI1_Handle();
}

void EXTI2_IRQHandler(void){
	gpio_clear_interrupt(2);
	EXTI2_Handle();
}

void EXTI3_IRQHandler(void){
	gpio_clear_interrupt(3);
	EXTI3_Handle();
}

void EXTI4_IRQHandler(void){
	gpio_clear_interrupt(4);
	EXTI4_Handle();
}

