#ifndef DMA_H
#define DMA_H
#include "stm32f10x.h"

#define DMA_CHANNEL_PRIORITY_LOW ((uint8_t)0x00)
#define DMA_CHANNEL_PRIORITY_MEDIUM ((uint8_t)0x01)
#define DMA_CHANNEL_PRIORITY_HIGH ((uint8_t)0x02)
#define DMA_CHANNEL_PRIORITY_VERY_HIGH ((uint8_t)0x03)

#define DMA_SIZE_8_BIT ((uint8_t)0x00)
#define DMA_SIZE_16_BIT ((uint8_t)0x01)
#define DMA_SIZE_32_BIT ((uint8_t)0x02)

#define DMA_MEMORY_TO_PERIPHERAL ((uint8_t)0x01)
#define DMA_PERIPHERAL_TO_MEMORY ((uint8_t)0x00)

typedef struct{
	DMA_TypeDef* dma;
	DMA_Channel_TypeDef* dma_channel;
	uint16_t channel_number;
	uint8_t memory_to_memory_mode;
	uint8_t priority;
	uint8_t memory_size;
	uint8_t peripheral_size;
	uint8_t memory_increment;
	uint8_t peripheral_increment;
	uint8_t circular_mode;
	uint8_t transfer_direction;
	uint16_t number_of_data;
	uint32_t peripheral_address;
	uint32_t memory_address;
}dma_data;

void dma_configure(dma_data* dma);
void dma_enable(dma_data* dma);
void dma_configure_transmission_complete_interrupt(dma_data* dma);
void dma_disable(dma_data* dma);
uint16_t dma_check_interrupt_transfer_complete(dma_data* dma);
void dma_clear_interrupt_flag(dma_data* dma);

#endif