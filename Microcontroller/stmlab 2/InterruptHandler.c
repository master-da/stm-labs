#include "InterruptHandler.h"
#include "Timer.h"

void USART2_IRQHandler(void){
	
	if (USART2->SR & USART_SR_RXNE) {
		
//		UART_GetString(USART2, 4, s);
//		UART_SendString(USART2, s);
		USART2->SR &= ~USART_SR_RXNE_Msk;
	}
}

void USART6_IRQHandler(void) {
	
	char p;
	
	if (USART6->SR & USART_SR_RXNE) {
//		UART_SendString(USART2, "USART  6 RXNE\n");
		
		p = (uint8_t)USART6->DR;

		USART6->DR = p;
		while (!(USART6->SR & USART_SR_TXE));
	}
}

void USART1_IRQHandler(void){
	
	char p;
	
	if (USART1->SR & USART_SR_RXNE) {
		UART_SendString(USART2, "\nUSART  1 RXNE\n");
		
		p = (uint8_t)USART1->DR;
		p++;
		UART_SendChar(USART2, p);
		
	}
}
