#ifndef GPIO
#define GPIO

#include "stm32f10x.h"

//pin modes
#define MODE_OUTPUT ((uint32_t)0x01)
#define MODE_INPUT ((uint32_t)0x02)

//input modes
#define INPUT_ANALOG ((uint32_t)0x00)
#define INPUT_FLOATING ((uint32_t)0x01)
#define INPUT_PULLUP ((uint32_t)0x02)
#define INPUT_PULLDOWN ((uint32_t)0x03)

//output modes
#define OUTPUT_GENERAL_PURPOSE_PUSH_PULL ((uint32_t)0x00)
#define OUTPUT_GENERAL_PURPOSE_OPEN_DRAIN ((uint32_t)0x01)
#define OUTPUT_ALTERNATE_FUNCTION_PUSH_PULL ((uint32_t)0x02)
#define OUTPUT_ALTERNATE_FUNCTION_OPEN_DRAIN ((uint32_t)0x03)

//output speeds
#define OUTPUT_SPEED_10MHz ((uint32_t)0x01)
#define OUTPUT_SPEED_2MHz ((uint32_t)0x02)
#define OUTPUT_SPEED_50MHz ((uint32_t)0x03)

//configuration bits
#define MODE0 ((uint8_t)0)
#define MODE1 ((uint8_t)1)
#define CNF0 ((uint8_t)2)
#define CNF1 ((uint8_t)3)

//Edge selection
#define RISING_EDGE ((uint32_t)0x00)
#define FALLING_EDGE ((uint32_t)0x01)
#define RISING_AND_FALLING_EDGES ((uint32_t)0x02)

//Interrupt port selection
#define PORTA_VALUE ((uint32_t)0x00)
#define PORTB_VALUE ((uint32_t)0x01)
#define PORTC_VALUE ((uint32_t)0x02)
#define PORTD_VALUE ((uint32_t)0x03)
#define PORTE_VALUE ((uint32_t)0x04)
#define PORTF_VALUE ((uint32_t)0x05)
#define PORTG_VALUE ((uint32_t)0x06)

typedef struct{
	GPIO_TypeDef* port;
	uint16_t pin;
} GPIO_Object;


/*************************************************************************************************
Configure function prototypes
*************************************************************************************************/

void gpio_enable_clock(GPIO_TypeDef* port);
void gpio_configure_pin_input(GPIO_Object* gpio, uint8_t mode);
void gpio_configure_pin_output(GPIO_Object* gpio, uint8_t mode, uint8_t speed);
void gpio_configure_interrupt(GPIO_Object* gpio, uint32_t edge, void(*handle)(void));

void gpio_write_pin(GPIO_Object* gpio, uint8_t state);
void gpio_toggle_pin(GPIO_Object* gpio);
uint32_t gpio_read_pin(GPIO_Object* gpio);
uint32_t gpio_read_port(GPIO_TypeDef* port);
void gpio_enable_interrupt(uint32_t pin);
void gpio_clear_interrupt(uint32_t pin);
void gpio_generate_software_interrupt(uint32_t pin);

//extern void (*EXTI0_Handler)(void);

void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI4_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void EXTI15_10_IRQHandler(void);


#endif

