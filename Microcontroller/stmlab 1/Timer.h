#include <stdint.h>
#include "GPIO.h"

#define TIM6_CLOCK ((uint32_t)0x0010)
#define TIM6_CR1_CEN ((uint32_t)0x0001)
#define TIM6_SR_UIF ((uint32_t)0x0001)

	
void TIM6_Config(void);
void delay_us (TIM_TypeDef* timer, uint16_t us);
void delay_ms (TIM_TypeDef* timer, uint16_t ms);
