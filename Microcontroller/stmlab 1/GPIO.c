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

#include "GPIO.h"
#include "CLOCK.h"

void GPIO_WritePin(GPIO_TypeDef *GPIOx, uint8_t GPIO_pin, GPIO_PinState PinState) {
	PinState != GPIO_PIN_RESET ? GPIOx->BSRR |= 1U << GPIO_pin : (GPIOx->BSRR |= 1U << (GPIO_pin + 16U));
}

void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_Init) {
	GPIOx->MODER &= ~(3U << (GPIO_Init->Pin * 2U)); 
	GPIOx->MODER |= ((GPIO_Init->Mode) << (GPIO_Init->Pin * 2U)); 
}

void enable_clock_for_register(uint16_t x) {
	RCC->AHB1ENR |= (1U << x);
}

GPIO_InitTypeDef get_init_type_def(uint32_t Pin, uint32_t Mode, 
				uint32_t Pull, uint32_t Speed, uint32_t Alternate) {
	GPIO_InitTypeDef init_type_def;
	init_type_def.Pin = Pin;
	init_type_def.Mode = Mode;
	init_type_def.Pull = Pull;
	init_type_def.Speed = Speed;
	init_type_def.Alternate = Alternate;
	return init_type_def;
}
