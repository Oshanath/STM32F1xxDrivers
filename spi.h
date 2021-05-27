#ifndef SPI
#define SPI

#include "stm32f10x.h"
#define SPI_CR1_BR_Pos ((uint8_t)0x03)

#define SPI_MODE_FULL_DUPLEX ((uint8_t)0x00)
#define SPI_MODE_HALF_DUPLEX_TRANSMIT_ONLY ((uint8_t)0x01)
#define SPI_MODE_HALF_DUPLEX_RECIEVE_ONLY ((uint8_t)0x02)
#define SPI_MODE_SIMPLEX ((uint8_t)0x03)

#define SPI_DATA_8_bit ((uint8_t)0x00)
#define SPI_DATA_16_bit ((uint8_t)0x01)

#define SPI_SLAVE_MANAGEMENT_SOFTWARE ((uint8_t)0x00)
#define SPI_SLAVE_MANAGEMENT_HARDWARE ((uint8_t)0x01)

#define TRANSMISSION_MODE_MSB_FIRST ((uint8_t)0x00)
#define TRANSMISSION_MODE_LSB_FIRST ((uint8_t)0x01)

#define SPI_CLOCK_2 ((uint8_t)0x00)
#define SPI_CLOCK_4 ((uint8_t)0x01)
#define SPI_CLOCK_8 ((uint8_t)0x02)
#define SPI_CLOCK_16 ((uint8_t)0x03)
#define SPI_CLOCK_32 ((uint8_t)0x04)
#define SPI_CLOCK_64 ((uint8_t)0x05)
#define SPI_CLOCK_128 ((uint8_t)0x06)
#define SPI_CLOCK_256 ((uint8_t)0x07)

#define SPI_CONFIGURATION_MASTER ((uint8_t)0x00)
#define SPI_CONFIGURATION_SLAVE ((uint8_t)0x01)

typedef struct{
	SPI_TypeDef* spi;
	uint8_t mode;
	uint8_t data_frame;
	uint8_t slave_management_mode;
	uint8_t transmission_mode;
	uint8_t baud_rate_control;
	uint8_t configuration;
	uint8_t clock_polarity;
	uint8_t clock_phase;
	uint8_t multi_master;
} SPI_data;

void spi_configure(SPI_data* data);
void spi_enable(SPI_data* data);
void spi_send(SPI_data* data, uint16_t value);


#endif

