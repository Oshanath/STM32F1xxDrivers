#include "adc.h"
#include "stm32f10x.h"
#include "delay.h"

void adc_configure(uint16_t conversions, uint32_t sequence[], uint16_t samples[]){

	RCC->CFGR |= RCC_CFGR_ADCPRE_1;
	RCC->APB2ENR |= (RCC_APB2ENR_AFIOEN | RCC_APB2ENR_IOPAEN | RCC_APB2ENR_ADC1EN);
	
	ADC1->CR1 |= ADC_CR1_EOCIE;
	NVIC_EnableIRQ(ADC1_2_IRQn);
	
	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR2 |= (ADC_CR2_ADON | ADC_CR2_CONT | ADC_CR2_DMA);
	
	enable_delay();
	delay(1);
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_CAL;
	
	while((ADC1->CR2 & ADC_CR2_CAL) == 1);
	
	ADC1->SQR1 |= (conversions - (unsigned)1) << 20;
	
	for(uint32_t i = 0; i < conversions; i++){
		
		if(i <= 9){
			ADC1->SMPR2 |= (unsigned)7 << (3 * i);
		}
		else{
			ADC1->SMPR1 |= (unsigned)7 << (3 * (i - 10));
		}
		
	
		if(i <= 5){
			ADC1->SQR3 |= (sequence[i] << (5 * i));
		}
		else if(i <= 11){
			ADC1->SQR2 |= sequence[i] << ((i - 6) * 5);
		}
		else{
			ADC1->SQR1 |= sequence[i] << ((i - 12) * 5);
		}
		
	}
	
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	DMA1_Channel1->CPAR = (uint32_t)(&ADC1->DR);
	DMA1_Channel1->CMAR = (uint32_t)(samples);
	DMA1_Channel1->CNDTR = conversions;
	DMA1_Channel1->CCR = (DMA_CCR1_CIRC | DMA_CCR1_MINC | DMA_CCR1_MSIZE_0 | DMA_CCR1_PSIZE_0);
	DMA1_Channel1->CCR |= DMA_CCR1_EN;
	
}

void ADC1_2_IRQHandler(void){

	int a = ADC1->DR;

}


