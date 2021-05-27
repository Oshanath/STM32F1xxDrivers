#include "Timer.h"
#include "GPIO.h"

static void (*TIM2_Handle)(void);
static void (*TIM3_Handle)(void);
static void (*TIM4_Handle)(void);
static void (*TIM5_Handle)(void);
static void (*TIM6_Handle)(void);
static void (*TIM7_Handle)(void);

void timer_configure(TIM_TypeDef* timer, timer_data* data){
	
	if(timer == TIM1){
		RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;
	}
	else if(timer == TIM8){
		RCC->APB2ENR |= RCC_APB2ENR_TIM8EN;
	}
	else if(timer == TIM2){
		RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	}
	else if(timer == TIM3){
		RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	}
	else if(timer == TIM4){
		RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
	}
	else if(timer == TIM5){
		RCC->APB1ENR |= RCC_APB1ENR_TIM5EN;
	}
	else if(timer == TIM6){
		RCC->APB1ENR |= RCC_APB1ENR_TIM6EN;
	}
	else if(timer == TIM7){
		RCC->APB1ENR |= RCC_APB1ENR_TIM7EN;
	}
	
	
	timer->ARR = data->auto_reload_value;
	if(data->direction == DIRECTION_DOWNCOUNTING){
		timer->CR1 |= TIM_CR1_DIR;
	}
	
	if(data->alignment == ALIGNMENT_CENTER_ALIGNED_UPCOUNTING_OUTPUT_COMPARE_INTERRUPT){
		TIM3->CR1 |= TIM_CR1_CMS_1;
	}
	else if(data->alignment == ALIGNMENT_CENTER_ALIGNED_DOWNCOUNTING_OUTPUT_COMPARE_INTERRUPT){
		TIM3->CR1 |= TIM_CR1_CMS_0;
	}
	else if(data->alignment == ALIGNMENT_CENTER_ALIGNED_UPCOUNTING_DOWNCOUNTING_OUTPUT_COMPARE_INTERRUPT){
		TIM3->CR1 |= TIM_CR1_CMS_0;
		TIM3->CR1 |= TIM_CR1_CMS_1;
	}
	
	timer->PSC = data->prescaler;
	timer->EGR |= TIM_EGR_UG;
	
}

void timer_enable(TIM_TypeDef* timer){
	timer->CR1 |= TIM_CR1_CEN;
}
void timer_configure_pwm(TIM_TypeDef* timer, timer_pwm_data* data){

	
	if(data->channel == 1){
		timer->CCER |= TIM_CCER_CC1E;
		timer->CCMR1 |= TIM_CCMR1_OC1PE;
		timer->CCR1 = data->compare_value;
		timer->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_2);
	}
	else if(data->channel == 2){
		timer->CCER |= TIM_CCER_CC2E;
		timer->CCMR1 |= TIM_CCMR1_OC2PE;
		timer->CCR2 = data->compare_value;
		timer->CCMR1 |= (TIM_CCMR1_OC2M_1 | TIM_CCMR1_OC2M_2);
	}
	else if(data->channel == 3){
		timer->CCER |= TIM_CCER_CC3E;
		timer->CCMR2 |= TIM_CCMR2_OC3PE;
		timer->CCR3 = data->compare_value;
		timer->CCMR2 |= (TIM_CCMR2_OC3M_1 | TIM_CCMR2_OC3M_2);
	}
	else if(data->channel == 4){
		timer->CCER |= TIM_CCER_CC4E;
		timer->CCMR2 |= TIM_CCMR2_OC4PE;
		timer->CCR4 = data->compare_value;
		timer->CCMR2 |= (TIM_CCMR2_OC4M_1 | TIM_CCMR2_OC4M_2);
	}
	
	if(data->pwm_mode == TIMER_CHANNEL_MODE_PWM_INVERTED){
		if(data->channel == 1){
			timer->CCER |= TIM_CCER_CC1P;
		}
		else if(data->channel == 2){
			timer->CCER |= TIM_CCER_CC2P;
		}
		else if(data->channel == 3){
			timer->CCER |= TIM_CCER_CC3P;
		}
		else if(data->channel == 4){
			timer->CCER |= TIM_CCER_CC4P;
		}
	}
	
	timer->CR1 |= TIM_CR1_ARPE;
	timer->EGR |= TIM_EGR_UG;
	
}

void timer_set_compare_value(TIM_TypeDef* timer, uint8_t channel, uint16_t compare_value){
	
	if(channel == 1){
		timer->CCR1 = compare_value;
	}
	else if(channel == 2){
		timer->CCR2 = compare_value;
	}
	else if(channel == 3){
		timer->CCR3 = compare_value;
	}
	else if(channel == 4){
		timer->CCR4 = compare_value;
	}
	
}


void timer_configure_interrupt(TIM_TypeDef* timer, void (*handle)(void)){

	timer->CR1 |= TIM_CR1_URS;
	timer->DIER |= TIM_DIER_UIE;
	
	if(timer == TIM2){
		NVIC_EnableIRQ(TIM2_IRQn);
		TIM2_Handle = handle;
	}
	else if(timer == TIM3){
		
		//gpio_write_pin(GPIOC, 7, 1);
		TIM3_Handle = handle;
		NVIC_EnableIRQ(TIM3_IRQn);
	}
	else if(timer == TIM4){
		NVIC_EnableIRQ(TIM4_IRQn);
		TIM4_Handle = handle;
	}
	else if(timer == TIM5){
		NVIC_EnableIRQ(TIM5_IRQn);
		TIM5_Handle = handle;
	}
	else if(timer == TIM6){
		NVIC_EnableIRQ(TIM6_IRQn);
		TIM6_Handle = handle;
	}
	else if(timer == TIM7){
		NVIC_EnableIRQ(TIM7_IRQn);
		TIM7_Handle = handle;
	}
	
}

void TIM2_IRQHandler(){
	TIM2->SR &= ~TIM_SR_UIF;
	TIM2_Handle();
}
void TIM3_IRQHandler(){
	TIM3->SR &= ~TIM_SR_UIF;
	TIM3_Handle();
}

void TIM4_IRQHandler(){
	TIM4->SR &= ~TIM_SR_UIF;
	TIM4_Handle();
}

void TIM5_IRQHandler(){
	TIM5->SR &= ~TIM_SR_UIF;
	TIM5_Handle();
}

void TIM6_IRQHandler(){
	TIM6->SR &= ~TIM_SR_UIF;
	TIM6_Handle();
}

void TIM7_IRQHandler(){
	TIM7->SR &= ~TIM_SR_UIF;
	TIM7_Handle();
	
}


