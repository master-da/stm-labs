#include "Timer.h"

void TIM6_Config(void) {
	RCC->APB1ENR |= TIM6_CLOCK; // Enable timer 6 clock
	
	TIM6->PSC = 90 - 1; // 90Hz/90 = 1MHz ~= 1uS delay
	TIM6->ARR = 0xFFFF; // Max APR value
	
	TIM6->CR1 |= TIM6_CR1_CEN; // enable the "coutner enable" bit
	
	while (!(TIM6->SR & TIM6_SR_UIF)); // wait till update interrupt flag is set. idk what it does but "updates on interrupts" is a safe guess
}

void delay_us(TIM_TypeDef* timer, uint16_t us) {
	timer->CNT = 0;
	while (timer->CNT < us);
}

void delay_ms(TIM_TypeDef* timer, uint16_t ms) {
	for (uint16_t i=0; i<ms; i++) delay_us(timer, 1000);
}
