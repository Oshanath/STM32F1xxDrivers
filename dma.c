#include "dma.h"

/*
*Please disable the dma each time the transmission is complete in memory to memory mode.
*/

void dma_configure(dma_data* dma){

	if(dma->dma == DMA1){
		RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	}
	else if(dma->dma == DMA2){
		RCC->AHBENR |= RCC_AHBENR_DMA2EN;
	}
	
	if(dma->memory_to_memory_mode){
		dma->dma_channel->CCR |= DMA_CCR1_MEM2MEM;
	}
	
	//set priority
	dma->dma_channel->CCR |= dma->priority << 12;
	
	//set memory and peripheral size
	dma->dma_channel->CCR |= (dma->memory_size << 10 | dma->peripheral_size << 8);
	
	if(dma->memory_increment){
		dma->dma_channel->CCR |= DMA_CCR1_MINC;
	}
	
	if(dma->peripheral_increment){
		dma->dma_channel->CCR |= DMA_CCR1_PINC;
	}
	
	if(dma->circular_mode){
		dma->dma_channel->CCR |= DMA_CCR1_CIRC;
	}
	
	if(dma->transfer_direction){
		dma->dma_channel->CCR |= DMA_CCR1_DIR;
	}
	
	//set number of data
	dma->dma_channel->CNDTR = dma->number_of_data;
	
	//set addresses
	dma->dma_channel->CPAR = dma->peripheral_address;
	dma->dma_channel->CMAR = dma->memory_address;
	
}

void dma_enable(dma_data* dma){
	dma->dma_channel->CNDTR = dma->number_of_data;
	dma->dma_channel->CCR |= DMA_CCR1_EN;
}

void dma_configure_transmission_complete_interrupt(dma_data* dma){
	if(dma->dma == DMA1){
		if(dma->dma_channel == DMA1_Channel1){
			NVIC_EnableIRQ(DMA1_Channel1_IRQn);
		}
		else if(dma->dma_channel == DMA1_Channel2){
			NVIC_EnableIRQ(DMA1_Channel2_IRQn);
		}
		else if(dma->dma_channel == DMA1_Channel3){
			NVIC_EnableIRQ(DMA1_Channel3_IRQn);
		}
		else if(dma->dma_channel == DMA1_Channel4){
			NVIC_EnableIRQ(DMA1_Channel4_IRQn);
		}
		else if(dma->dma_channel == DMA1_Channel5){
			NVIC_EnableIRQ(DMA1_Channel5_IRQn);
		}
		else if(dma->dma_channel == DMA1_Channel6){
			NVIC_EnableIRQ(DMA1_Channel6_IRQn);
		}
		else if(dma->dma_channel == DMA1_Channel7){
			NVIC_EnableIRQ(DMA1_Channel7_IRQn);
		}
	}
	else if(dma->dma == DMA2){
		if(dma->dma_channel == DMA2_Channel1){
			NVIC_EnableIRQ(DMA2_Channel1_IRQn);
		}
		else if(dma->dma_channel == DMA2_Channel2){
			NVIC_EnableIRQ(DMA2_Channel2_IRQn);
		}
		else if(dma->dma_channel == DMA2_Channel3){
			NVIC_EnableIRQ(DMA2_Channel3_IRQn);
		}
		else if(dma->dma_channel == DMA2_Channel4 || dma->dma_channel == DMA2_Channel5){
			NVIC_EnableIRQ(DMA2_Channel4_5_IRQn);
		}
	}
	
	dma->dma_channel->CCR |= DMA_CCR1_TCIE;
	
}

void dma_disable(dma_data* dma){
	dma->dma_channel->CCR &= ~DMA_CCR1_EN;
}

uint16_t dma_check_interrupt_transfer_complete(dma_data* dma){
	return dma->dma->ISR & (1 << ((dma->channel_number - 1) * 4 + 1));
}

void dma_clear_interrupt_flag(dma_data* dma){
	dma->dma->IFCR |= (1 << ((dma->channel_number - 1) * 4 + 1));
}

