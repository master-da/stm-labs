#include "InterruptHandler.h"

void USART2_IRQHandler(void){
	
	char s[100]; 
	
	UART_SendString(USART2, "USART2 Interrupt\n");
	if (USART2->SR & USART_SR_RXNE) {
		UART_SendString(USART2, "USART2 Interrupt\n");
		
		UART_GetString(USART2, 4, s);
		UART_SendString(USART2, s);
		USART2->SR &= ~USART_SR_RXNE_Msk;
	}
}

//void USART6_IRQHandler(void) {
//	UART_SendString(USART2, "USART6 Interrupt\n");
//	if (USART6->SR & USART_SR_RXNE) {
//		
//		char s[100];
//		UART_GetString(USART6, 4, s);
//		for (int i=0; i<4; i++) 
//			s[i]++;
//		UART_SendString(USART6, s);
//		
//		USART6->SR &= ~USART_SR_RXNE_Msk;
//	}
//}

//void USART1_IRQHandler(void){
//	UART_SendString(USART2, "USART1 Interrupt");
//	if (USART1->SR & USART_SR_RXNE) {
//		UART_SendString(USART2, "USART1 Interrupt\n");
//		char s[100];
//		UART_GetString(USART1, 4, s);
//		
//		UART_SendString(USART2, s);
//		USART1->SR &= USART_SR_RXNE_Msk;
//	}
//}
