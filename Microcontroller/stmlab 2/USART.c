/*
 * Copyright (c) 2022 
 * Computer Science and Engineering, University of Dhaka
 * Credit: CSE Batch 25 (starter) and Prof. Mosaddek Tushar
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE UNIVERSITY AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE UNIVERSITY OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include "USART.h"

void _USART_WRITE(USART_TypeDef *usart,uint8_t *s)
{
	while (*s) UART_SendChar(usart,*s++);
}
/*****Modify according to your need *****/
uint8_t _USART_READ(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		if(i==0 && buff[i]!= 0x03F){continue;}
		n=i;
	}
	return n;
}

uint8_t _USART_READ_STR(USART_TypeDef* usart,uint8_t *buff,uint16_t size)
{
	uint8_t n=0;
	for(uint8_t i=0;i<size;i++){
		buff[i]=UART_GetChar(usart);
		n=i;
		if(buff[i]=='\0' || buff[i] == '\n' || buff[i] == ' ')
		{ 	
			buff[i]='\0';
			break;
		}
	}
	return n;
}


void UART_SendChar(USART_TypeDef *usart,uint8_t c){
	usart->DR = c;
	while(!(usart->SR & USART_SR_TXE));
}

// Send a string to the UART PORTx
void UART_SendString(USART_TypeDef *usart,const char *s){
	while (*s) {UART_SendChar(usart,*s);s++;}
}

uint8_t UART_GetChar(USART_TypeDef *usart){
	uint8_t tmp;
	while(!(usart->SR & USART_SR_RXNE));
	tmp=(uint8_t)usart->DR;
	return tmp;
}
void UART_GetString(USART_TypeDef *uart,uint16_t size,uint8_t* buff)
{
	uint16_t i=0;
	while(size--)
	{
		uint8_t x=UART_GetChar(uart);
		buff[i]=x;
		i++;
	}
	buff[i]='\0';
		
}

void startTransmissionFromUARTBuff(USART_TypeDef* usart, const char* str) {
	usart->CR1 |= USART_CR1_TXEIE;
	usart->DR = str[0];
}

void stopTransmissionFromUARTBuff(USART_TypeDef* usart) {
	usart->CR1 &= ~(USART_CR1_TXEIE);
}


/**********************************
* USART 2 Configuration
***********************************/
void UART2_Config(bool enableReceiveInterupt){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN; //enable UART 2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; //enable GPIOA clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOA->MODER |= GPIO_MODER_MODER2_1;//(2<<4); //bits [5:4] -> 1:0 -->Alternate function for pin PA2
	GPIOA->MODER |= GPIO_MODER_MODER3_1;//(2<<6); //bits [7:6] -> 1:0 -->Alternate function for PA3
	
	GPIOA->OSPEEDR |= GPIO_MODER_MODER2 | GPIO_MODER_MODER3;// (3<<4) | (3<<6); //bits [5:4] -> 1:1 -> high speed PA2; bits [7:6] -> 1:1 -> high speed PA3 
	
	GPIOA->AFR[0] |= (7<<GPIO_AFRL_AFSEL2_Pos);//Bytes (11:10:09:08) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA2
	GPIOA->AFR[0] |= (7<<GPIO_AFRL_AFSEL3_Pos); //Bytes (15:14:13:12) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA3
	
	//3. Enable UART on USART_CR1 rgister
	USART2->CR1 = 0x00; //clear USART
	USART2->CR1 |= USART_CR1_UE;  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART2->CR1 &= ~USART_CR1_M; // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART2->BRR |= (7<<USART_BRR_DIV_Fraction_Pos) | (24<<USART_BRR_DIV_Mantissa_Pos); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART2->CR1 |= USART_CR1_RE; // enable RE for receiver 
	USART2->CR1 |= USART_CR1_TE; //enable TE for transmitter
	
	if (enableReceiveInterupt) {
		NVIC_EnableIRQ(USART2_IRQn);
		NVIC_SetPriority(USART2_IRQn, 3);
		USART2->CR1 |= USART_CR1_RXNEIE;
	}
	
}


/**********************************
* USART 1 Configuration
***********************************/

void UART1_Config(bool enableReceiveInterupt){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_USART1EN; //enable UART 1
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN; //enable GPIOBA clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOB->MODER |= GPIO_MODER_MODER6_1; //bits [19:18] -> 1:0 -->Alternate function for pin PB6
	GPIOB->MODER |= GPIO_MODER_MODER7_1; //bits [21:20] -> 1:0 -->Alternate function for PB7
	
	GPIOB->OSPEEDR |= GPIO_MODER_MODER6 | GPIO_MODER_MODER7; //bits [19:18] -> 1:1 -> high speed PB6; bits [21:20] -> 1:1 -> high speed PB7
	
	GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL6_Pos);// AF7 Alternate function for USART1 at pin PB6
	GPIOB->AFR[0] |= (7<<GPIO_AFRL_AFSEL7_Pos); // AF7 Alternate function for USART1 at pin PB7
	
	//3. Enable UART on USART_CR1 rgister
	USART1->CR1 = 0x00; //clear USART
	USART1->CR1 |= USART_CR1_UE;  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART1->CR1 &= ~USART_CR1_M; // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART1->BRR |= (7<<USART_BRR_DIV_Fraction_Pos) | (24<<USART_BRR_DIV_Mantissa_Pos); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART1->CR1 |= USART_CR1_RE; // enable RE for receiver 
	USART1->CR1 |= USART_CR1_TE; //enable TE for transmitter
	
	if (enableReceiveInterupt) {
		NVIC_EnableIRQ(USART1_IRQn);
		NVIC_SetPriority(USART1_IRQn, 2);
		USART1->CR1 |= USART_CR1_RXNEIE;
	}
}

/********************************t**
* USART 6 Configuration
***********************************/

void UART6_Config(bool enableReceiveInterupt){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB2ENR |= RCC_APB2ENR_USART6EN; //enable UART 6
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; //enable GPIOC clock
	
	//2. Configure UART pin for Alternate function
	GPIOC->MODER |= GPIO_MODER_MODE6_1; //bits [13:12] -> 1:0 -->Alternate function for pin PC6
	GPIOC->MODER |= GPIO_MODER_MODE7_1; //bits [15:14] -> 1:0 -->Alternate function for PC7
	
	GPIOC->OSPEEDR |= GPIO_MODER_MODE6 | GPIO_MODER_MODE7; //bits [13:12] -> 1:1 -> high speed PC6; bits [15:14] -> 1:1 -> high speed PC7
	
	GPIOC->AFR[0] |= (8<<GPIO_AFRL_AFSEL6_Pos);//Bytes (27:26:25:24) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PC6
	GPIOC->AFR[0] |= ((unsigned)8<<GPIO_AFRL_AFSEL7_Pos); //Bytes (31:30:29:28) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PC7
	
	//3. Enable UART on USART_CR1 rgister
	USART6->CR1 = 0x00; //clear USART
	USART6->CR1 |= USART_CR1_UE;  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART6->CR1 &=~USART_CR1_M; // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART6->BRR |= (7<<USART_BRR_DIV_Fraction_Pos) | (24<<USART_BRR_DIV_Mantissa_Pos); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART6->CR1 |= USART_CR1_RE; // enable RE for receiver 
	USART6->CR1 |= USART_CR1_TE; //enable TE for transmitter
	
	if (enableReceiveInterupt){
		NVIC_EnableIRQ(USART6_IRQn);
		NVIC_SetPriority(USART6_IRQn, 1);
		USART6->CR1 |= USART_CR1_RXNEIE;
	}	
}
