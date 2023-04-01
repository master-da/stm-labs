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
#include <stdint.h>
#include <stdio.h>
#include "CLOCK.h"
#include "stm32f446xx.h"
#include <string.h>
#include "global.h"
#include "SYS_INIT.h"



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
	
	while(!(usart->SR & (1<<7)));
}

// Send a string to the UART PORTx
void UART_SendString(USART_TypeDef *usart,const char *s){
	while (*s) {UART_SendChar(usart,*s);s++;}
	UART_SendChar(usart,'\n');
	
}

uint8_t UART_GetChar(USART_TypeDef *usart){
	uint8_t tmp;
	while(!(usart->SR & (1<<5)));
	tmp=(uint8_t)usart->DR;
	return tmp;
}
void UART_GetString(USART_TypeDef *uart, uint8_t* buff) {
    uint16_t i = 0;
    uint8_t x = UART_GetChar(uart);
    while (x != '\0') {
        buff[i] = x;
        i++;
        x = UART_GetChar(uart);
    }
    buff[i] = '\0';
}

void UART2_SendString(char *s){
	con_out=0;
	strcpy(con_buffer_outp, s);

	int i=0;
	/*for( i=0; i<strlen(s); i++)
	{
		con_buffer_outp[i]=s[i];

	}*/
	

	for( i=0; i<strlen(s); i++)
	{
		
		USART2->CR1 |= USART_CR1_TXEIE;
		//while(USART2->CR1 & USART_CR1_TXEIE);
		con_out++;
	}
	//UART_SendString(USART2,con_buffer_outp);
	//strcpy(con_buffer_outp, "");
	//strcpy(s, "");


	
}

/**********************************
* USART 2 Configuration
***********************************/
void UART2_Config(void){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB1ENR |= (1<<17); //enable UART 2
	RCC->AHB1ENR |= (1<<0); //enable GPIOA clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOA->MODER |= (2<<4); //bits [5:4] -> 1:0 -->Alternate function for pin PA2
	GPIOA->MODER |= (2<<6); //bits [7:6] -> 1:0 -->Alternate function for PA3
	
	GPIOA->OSPEEDR |= (3<<4) | (3<<6); //bits [5:4] -> 1:1 -> high speed PA2; bits [7:6] -> 1:1 -> high speed PA3 
	//pa2 tx
	//pa3 rx
	GPIOA->AFR[0] |= (7<<8);//Bytes (11:10:09:08) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA2
	GPIOA->AFR[0] |= (7<<12); //Bytes (15:14:13:12) = 0:1:1:1 --> AF7 Alternate function for USART2 at pin PA3
	
	//3. Enable UART on USART_CR1 rgister
	USART2->CR1 = 0x00; //clear USART
	USART2->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART2->CR1 &= ~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART2->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART2->CR1 |= (1<<2); // enable RE for receiver 
	USART2->CR1 |= (1<<3); //enable TE for transmitter
	
	NVIC_EnableIRQ(USART2_IRQn);
	USART2->CR1 |= USART_CR1_RXNEIE;
	
}
/**********************************
* USART 1 Configuration
***********************************/
void UART1_Config(void){
    /******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB2ENR |= (1<<4); //enable UART 1
	RCC->AHB1ENR |= (1<<1); //enable GPIOB clock
	
		
	//2. Configure UART pin for Alternate function
	GPIOB->MODER |= (2<<12); //bits [13:12] -> 1:0 -->Alternate function for pin PB6
	GPIOB->MODER |= (2<<14); //bits [15:14] -> 1:0 -->Alternate function for PB7
	
	GPIOB->OSPEEDR |= (3<<12) | (3<<14); //bits [13:12] -> 1:1 -> high speed PB6; bits [15:14] -> 1:1 -> high speed PB7
	//pb6 tx
		//pb7 rx
	GPIOB->AFR[0] |= (7<<24);//Bytes (27:26:25:24) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PB6
	GPIOB->AFR[0] |= (7<<28); //Bytes (31:30:29:28) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PB7
	
	//3. Enable UART on USART_CR1 rgister
	USART1->CR1 = 0x00; //clear USART
	USART1->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART1->CR1 &=~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART1->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART1->CR1 |= (1<<2); // enable RE for receiver 
	USART1->CR1 |= (1<<3); //enable TE for transmitter
	
	NVIC_EnableIRQ(USART1_IRQn);
	USART1->CR1 |= USART_CR1_RXNEIE;
	
}

/**********************************
* USART 6 Configuration
***********************************/

void UART6_Config(void){
	/******************************************
	* 1. Enable UART clock and GPIO clock
	* 2. Configure UART pin for Alternate function
	* 3. Enable UART on USART_CR1 rgister
	* 4. Program M bit in USART CR1 to define the word length
	* 5. Select the baud rate using the USART_BRR register.
	* 6. Enable transmission TE and recieption bits in USART_CR1 register
	*******************************************/
	//1. Enable UART clock and GPIO clock
	RCC->APB2ENR |= (1<<5); //enable UART 6
	RCC->AHB1ENR |= (1<<2); //enable GPIOC clock
	
		//pc6 tx
		//pc7 rx
	//2. Configure UART pin for Alternate function
	GPIOC->MODER |= (2<<12); //bits [13:12] -> 1:0 -->Alternate function for pin PC6
	GPIOC->MODER |= (2<<14); //bits [15:14] -> 1:0 -->Alternate function for PC7
	
	GPIOC->OSPEEDR |= (3<<12) | (3<<14); //bits [13:12] -> 1:1 -> high speed PC6; bits [15:14] -> 1:1 -> high speed PC7
	
	GPIOC->AFR[0] |= (8<<24);//Bytes (27:26:25:24) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PC6
	GPIOC->AFR[0] |= (8<<28); //Bytes (31:30:29:28) = 0:1:1:1 --> AF7 Alternate function for USART1 at pin PC7
	
	//3. Enable UART on USART_CR1 rgister
	USART6->CR1 = 0x00; //clear USART
	USART6->CR1 |= (1<<13);  // UE-bit enable USART
	
	//4. Program M bit in USART CR1 to define the word length
	USART6->CR1 &=~(1U<<12); // set M bit  = 0 for 8-bit word length
	
	//5. Select the baud rate using the USART_BRR register.
	USART6->BRR |= (7<<0) | (24<<4); //115200
	
	//  6. Enable transmission TE and recieption bits in USART_CR1 register
	USART6->CR1 |= (1<<2); // enable RE for receiver 
	USART6->CR1 |= (1<<3); //enable TE for transmitter
	
	NVIC_EnableIRQ(USART6_IRQn);
	USART6->CR1 |= USART_CR1_RXNEIE;
	
}

