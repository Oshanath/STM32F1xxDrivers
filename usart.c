#include "usart.h"
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "delay.h"
#include "GPIO.h"

//sampel code for serial monitor
//recieve interrupts are also activated
//serial_monitor = usart_configure_serial_monitor(9600);
//usart_print(&serial_monitor, "Hello");

void usart_configure(usart_data* usart){
	
	uint32_t clock_rate;
	
	if(usart->usart == USART1){
		RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
		clock_rate = 72000000;
	}
	else if(usart->usart == USART2){
		RCC->APB1ENR |= RCC_APB1ENR_USART2EN;
		clock_rate = 36000000;
	}
	else if(usart->usart == USART3){
		RCC->APB1ENR |= RCC_APB1ENR_USART3EN;
		clock_rate = 36000000;
	}
	
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	volatile double usart_div = ((double)clock_rate / (16 * usart->baud_rate));
	uint16_t usart_div_mantissa = (uint16_t)usart_div;
	uint8_t usart_div_fraction = (uint8_t)((usart_div - usart_div_mantissa) * 16);
	
	usart->usart->BRR |= usart_div_mantissa << 4;
	usart->usart->BRR |= usart_div_fraction;
	
	if(usart->world_length == USART_WORD_LENGTH_9_BIT){
		usart->usart->CR1 |= USART_CR1_M;
	}
	
	if(usart->parity == USART_PARITY_EVEN || usart->parity == USART_PARITY_ODD){
		usart->usart->CR1 |= USART_CR1_PCE;
		
		if(usart->parity == USART_PARITY_ODD){
			usart->usart->CR1 |= USART_CR1_PS;
		}
	}
	
	usart->usart->CR1 |= (USART_CR1_TE | USART_CR1_RE);
	
	if(usart->dma_recieve){
		usart->usart->CR3 |= USART_CR3_DMAR;
	}
	
	if(usart->dma_transmit){
		usart->usart->CR3 |= USART_CR3_DMAT;
	}
	
}

void usart_enable(usart_data* usart){
	usart->usart->CR1 |= USART_CR1_UE;
}

void usart_send(usart_data* usart, uint16_t data){
	usart->usart->DR  = data;
}

void usart_print(usart_data* usart, char* message, ...){

	char buffer[80];
	
	va_list args;
	va_start(args, message);
	vsprintf(buffer, message, args);
	
	for(int i = 0; i < strlen(buffer); i++){
		usart_send(usart, buffer[i]);
		while(!(usart->usart->SR & USART_SR_TXE));
	}
	
}

void usart_enable_interrupts(usart_data* usart){
	if(usart->usart == USART1){
		NVIC_EnableIRQ(USART1_IRQn);
	}
	else if(usart->usart == USART2){
		NVIC_EnableIRQ(USART2_IRQn);
	}
	else if(usart->usart == USART3){
		NVIC_EnableIRQ(USART3_IRQn);
	}
	
	usart->usart->SR &= ~USART_SR_TC;
	
}

void usart_configure_recieved_interrupt(usart_data* usart){
	usart_enable_interrupts(usart);
	usart->usart->CR1 |= USART_CR1_RXNEIE;
}

void usart_configure_transmit_buffered_interrupt(usart_data* usart){
	usart_enable_interrupts(usart);
	usart->usart->CR1 |= USART_CR1_TXEIE;
}

void usart_configure_transmit_interrupt(usart_data* usart){
	usart_enable_interrupts(usart);
	usart->usart->CR1 |= USART_CR1_TCIE;
}

uint16_t usart_is_recieved(usart_data* usart){
	return usart->usart->SR & USART_SR_RXNE;
}

uint16_t usart_is_transmission_buffered(usart_data* usart){
	return usart->usart->SR & USART_SR_TXE;
}

uint16_t usart_is_transmit(usart_data* usart){
	return usart->usart->SR & USART_SR_TC;
}

usart_data usart_configure_serial_monitor(uint16_t baud_rate){
	GPIO_Object tx = {.port = GPIOB, .pin = 6};
	GPIO_Object rx = {.port = GPIOB, .pin = 7};
	usart_data usart = {.baud_rate = 9600, .usart = USART1};
	
	gpio_enable_clock(GPIOB);
	gpio_configure_pin_output(&tx, OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL, OUTPUT_SPEED_50MHz);
	gpio_configure_pin_input(&rx, INPUT_FLOATING);
	usart_configure(&usart);
	AFIO->MAPR |= AFIO_MAPR_USART1_REMAP;
	usart_configure_recieved_interrupt(&usart);
	usart_enable(&usart);
	return usart;
	
}

