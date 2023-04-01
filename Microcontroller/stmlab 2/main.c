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
	
	char s[] = "This is for USART1";
	
	srand(69);
	initClock();
	sysInit();
	TIM6_Config();
	
	UART2_Config(true);
	UART1_Config(true);
	UART6_Config(true);
	
//	UART_SendString(USART2, "Hello\n");
	
	USART1->DR = 'P';
//	UART_SendString(USART2, "sent\n");
	while (!(USART1->SR & USART_SR_TXE));
	USART1->DR = 'Q';
	UART_SendString(USART2, "sent\n");
	
	
}
