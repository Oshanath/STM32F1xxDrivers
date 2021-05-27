#ifndef Timer
#define Timer
#include "stm32f10x.h"

//alignments
#define ALIGNMENT_EDGE_ALIGNED ((uint8_t)0x00)
#define ALIGNMENT_CENTER_ALIGNED_DOWNCOUNTING_OUTPUT_COMPARE_INTERRUPT ((uint8_t)0x01)
#define ALIGNMENT_CENTER_ALIGNED_UPCOUNTING_OUTPUT_COMPARE_INTERRUPT ((uint8_t)0x02)
#define ALIGNMENT_CENTER_ALIGNED_UPCOUNTING_DOWNCOUNTING_OUTPUT_COMPARE_INTERRUPT ((uint8_t)0x03)

//directions
#define DIRECTION_UPCOUNTING ((uint8_t)0x00)
#define DIRECTION_DOWNCOUNTING ((uint8_t)0x01)

//channel modes
#define TIMER_CHANNEL_MODE_PWM_NORMAL ((uint8_t)0x00)
#define TIMER_CHANNEL_MODE_PWM_INVERTED ((uint8_t)0x01)

void TIM2_IRQHandler(void);
void TIM3_IRQHandler(void);
void TIM4_IRQHandler(void);
void TIM5_IRQHandler(void);
void TIM6_IRQHandler(void);
void TIM7_IRQHandler(void);

typedef struct{
	uint16_t auto_reload_value;
	uint16_t prescaler;
	uint8_t alignment;
	uint8_t direction;
} timer_data;

typedef struct{
	uint8_t channel;
	uint16_t compare_value;
	uint8_t pwm_mode;
} timer_pwm_data;

void timer_configure(TIM_TypeDef* timer, timer_data* data);
void timer_configure_pwm(TIM_TypeDef* timer, timer_pwm_data* data);
void timer_set_compare_value(TIM_TypeDef* timer, uint8_t channel, uint16_t compare_value);
void timer_configure_interrupt(TIM_TypeDef* timer, void (*handle)(void));
void timer_enable(TIM_TypeDef* timer);

#endif

