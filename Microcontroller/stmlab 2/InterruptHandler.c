#include "InterruptHandler.h"

void USART2_IRQHandler(void){
	
	if ((USART2->CR1 & USART_CR1_RXNEIE) && (USART2->SR & USART_SR_RXNE)) {
		
		UART2_buff[UART2_buff_write_pos] = (uint8_t) USART2->DR;
		
		if (UART2_buff[UART2_buff_write_pos] == '\0') {
			
			USART1->CR1 |= USART_CR1_TXEIE;
			strcpy(UART1_buff, UART2_buff);
			USART1->DR = UART1_buff[0];
			UART1_buff_write_pos = 1;
			
			UART2_buff_write_pos = 0;
			
		} else {
			UART2_buff_write_pos++;
		}
		
	}
}

void USART6_IRQHandler(void) {
	
	if ((USART6->CR1 & USART_CR1_RXNEIE) && (USART6->SR & USART_SR_RXNE)) {
		
		UART6_buff[UART6_buff_write_pos] = (uint8_t) USART6->DR;
	
		if (UART6_buff[UART6_buff_write_pos] == '\0') {
			
			UART6_buff_write_pos = 0;
			modifyString(UART6_buff);
			
			USART6->CR1 |= USART_CR1_TXEIE;
			USART6->DR = UART6_buff[0];
			UART6_buff_write_pos = 1;
			UART1_buff_write_pos = 0;
			
		} else {
			UART6_buff_write_pos++;
		}
	}
		if ((USART6->CR1 & USART_CR1_TXEIE) && (USART6->SR & USART_SR_TXE)) {

			if (UART6_buff[UART6_buff_write_pos] == '\0') 
				USART6->CR1 &= ~(USART_CR1_TXEIE);
			
			
			USART6->DR = UART6_buff[UART6_buff_write_pos];
			UART6_buff_write_pos++;
		}
}

void USART1_IRQHandler(void){
	
	if ((USART1->CR1 & USART_CR1_RXNEIE) && (USART1->SR & USART_SR_RXNE)) {
		
		char p = (uint8_t)USART1->DR;
		UART_SendChar(USART2, ++p);
		
	}
	
	if ((USART1->CR1 & USART_CR1_TXEIE) && (USART1->SR & USART_SR_TXE)) {
		
		if (UART1_buff[UART1_buff_write_pos] == '\0') 
			USART1->CR1 &= ~(USART_CR1_TXEIE);
		
		
		USART1->DR = UART1_buff[UART1_buff_write_pos];
		UART1_buff_write_pos++;
	}
}
