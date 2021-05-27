#include "spi.h" 

void spi_configure(SPI_data* data){

	//enable clock for spi
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN;
	RCC->APB2ENR |= RCC_APB2ENR_AFIOEN;
	
	//SPI Mode
	if(data->mode == SPI_MODE_FULL_DUPLEX){
		
	}
	else if(data->mode == (SPI_MODE_HALF_DUPLEX_RECIEVE_ONLY || SPI_MODE_HALF_DUPLEX_TRANSMIT_ONLY)){
		data->spi->CR1 |= SPI_CR1_BIDIMODE;
	}
	else if(data->mode == SPI_MODE_SIMPLEX){
		data->spi->CR1 |= SPI_CR1_RXONLY;
	}
	
	if(data->mode == SPI_MODE_HALF_DUPLEX_TRANSMIT_ONLY){
		data->spi->CR1 |= SPI_CR1_BIDIOE;
	}
	
	
	//SPI data frame format
	
	if(data->data_frame == SPI_DATA_16_bit){
		data->spi->CR1 |= SPI_CR1_DFF;
	}
	
	//slave management
	if(data->slave_management_mode == SPI_SLAVE_MANAGEMENT_SOFTWARE){
		data->spi->CR1 |= SPI_CR1_SSM;
	}
	
	//transmission mode
	if(data->transmission_mode == TRANSMISSION_MODE_LSB_FIRST){
		data->spi->CR1 |= SPI_CR1_LSBFIRST;
	}
	
	//baud control
	data->spi->CR1 |= (data->baud_rate_control << SPI_CR1_BR_Pos);
	
	//configureation
	if(data->configuration == SPI_CONFIGURATION_MASTER){
		data->spi->CR1 |= SPI_CR1_MSTR;
	}
	
	//clock polarty and phase
	if(data->clock_polarity){
		data->spi->CR1 |= SPI_CR1_CPOL;
	}
	
	if(data->clock_phase){
		data->spi->CR1 |= SPI_CR1_CPHA;
	}
	
	if(data->multi_master == 0){
		data->spi->CR2 |= SPI_CR2_SSOE;
	}

}

void spi_enable(SPI_data* data){
	data->spi->CR1 |= SPI_CR1_SPE;
}

void spi_send(SPI_data* data, uint16_t value){

	data->spi->DR = value;

}

