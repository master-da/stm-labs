#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "stm32f446xx.h"
#include "stm32f4xx.h"
#include "SYS_INIT.h"
#include "CLOCK.h"
#include "GPIO.h"
#include "Timer.h"
#include "USART.h"
#include "InterruptHandler.h"

int main(void) {
	
	srand(69);
	initClock();
	sysInit();
	TIM6_Config();
	
	UART2_Config(true);
	UART1_Config(true);
	UART6_Config(true);
	
	while (1) {
	}
	
}
