#ifndef USART_H
#define USART_H

#include "stm32f10x.h"

#define USART_WORD_LENGTH_8_BIT ((uint8_t)0x00)
#define USART_WORD_LENGTH_9_BIT ((uint8_t)0x01)

#define USART_PARITY_DISABLED ((uint8_t)0x00)
#define USART_PARITY_EVEN ((uint8_t)0x01)
#define USART_PARITY_ODD ((uint8_t)0x02)

typedef struct{
	USART_TypeDef* usart;
	uint32_t baud_rate;
	uint8_t world_length;
	uint8_t parity;
	uint8_t dma_recieve;
	uint8_t dma_transmit;
} usart_data;

void usart_configure(usart_data* usart);
void usart_enable(usart_data* usart);
void usart_send(usart_data* usart, uint16_t data);
void usart_print(usart_data* usart, char* message, ...);
void usart_enable_interrupts(usart_data* usart);
void usart_configure_transmit_interrupt(usart_data* usart);
void usart_configure_transmit_buffered_interrupt(usart_data* usart);
void usart_configure_recieved_interrupt(usart_data* usart);
uint16_t usart_is_recieved(usart_data* usart);
uint16_t usart_is_transmission_buffered(usart_data* usart);
uint16_t usart_is_transmit(usart_data* usart);
usart_data usart_configure_serial_monitor(uint16_t baud_rate);

#endif