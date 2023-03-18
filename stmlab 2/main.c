#include "stm32f446xx.h"
#include "stm32f4xx.h"
#include "SYS_INIT.h"
#include "CLOCK.h"
#include "GPIO.h"
#include "Timer.h"
#include "USART.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(void) {
	
	srand(69);
	initClock();
	sysInit();
	TIM6_Config();
	UART2_Config();
	
	while(1){
		UART_SendChar(USART2, 'B');
		delay_ms(TIM6, 1000);
	}
}
