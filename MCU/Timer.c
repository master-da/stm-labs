#include "CLOCK.h"
#include "USART.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "Timer.h"
#include "global.h"
#include <stdio.h>
#include <string.h>
#include "InterruptHandler.h"
#include <stdlib.h>
uint32_t counter = 0;

//gpioc input D 4, U 8,L 5.R 1, LR_RED 9,LR green 10,UD_green 11,UD_red 12

int pin_stat(GPIO_TypeDef* GPIOx,uint16_t pin)
{
	return(GPIOx->IDR & (1<<pin));
}
int get_val(uint16_t pin)
{
	if(pin_stat(GPIOC,pin)) return 1;
	else return 0;
}
char prev2[500]="";
char prev3[500]="";
char tmp1[500];
char tmp2[500];
char tmp3[500];
char tmp4[500];
char tim1[500];
char tim2[500];
char tim3[500];
char tim4[500];
char sen[1000];


void monitor_signal()
{
	strcpy(tmp1," traffic light 1 ");
	strcpy(tmp2," traffic light 2 ");
	
	int UDR=get_val(12),UDG=get_val(11);
	int LRR=get_val(9),LRG=get_val(10);
	
	if(UDR==1) strcat(tmp1," ON "); //ud red
	else strcat(tmp1," OFF ");
	
	if(UDR==0 && UDG==0) strcat(tmp1," ON "); //ud yellow
	else strcat(tmp1," OFF ");
	
	if(UDG==1) strcat(tmp1," ON\n"); //UD green
	else strcat(tmp1," OFF\n");
	
	
	if(LRR==1) strcat(tmp2," ON "); //LR red
	else strcat(tmp2," OFF ");
	
	if(LRR==0 && LRG==0) strcat(tmp2," ON "); //LR yellow
	else strcat(tmp2," OFF ");
	
	if(LRG==1) strcat(tmp2," ON\n"); //LR green
	else strcat(tmp2," OFF\n");
	
	
}
void monitor_cong()
{
	int data_D=get_val(4), data_U=get_val(8);
	int data_L=get_val(5), data_R=get_val(1);
	if(data_D==1 || data_U==1) strcpy(tmp3," road north south heavy traffic\n");
	else strcpy(tmp3," road north south light traffic\n");
	
	if(data_R==1 || data_L==1) strcpy(tmp4," road east west heavy traffic\n");
	else strcpy(tmp4," road east west light traffic\n");
}
void print_monitor()
{
						monitor_signal();
						monitor_cong();
					//	UART_SendString(USART2,"-----------------------------------");

						//UART_SendString(USART2,prev3);
						//UART_SendString(USART2,prev2);

						sprintf(tim1, "%d", counter);
						sprintf(tim2, "%d", counter);
						sprintf(tim3, "%d", counter);
						sprintf(tim4, "%d", counter);
							
						strcat(tim1,tmp1);
						strcat(tim2,tmp2);
						strcat(tim3,tmp3);
						strcat(tim4,tmp4);
						
						strcat(tim1,tim2);
						strcat(tim1,tim3);
						strcat(tim1,tim4);

						strcpy(sen,prev3);
						strcat(sen,prev2);
						strcat(sen,tim1);
						tim_flag=1;
						//UART_SendString(USART2,sen);
						//light_to_control(sen);
						strcpy(prev3,prev2);
						strcpy(prev2,tim1);
						//UART_SendString(USART2,"-----------------------------------");

}



void TIM6_DAC_IRQHandler(void) {
    if (TIM6->SR & TIM_SR_UIF) { // Check if timer update interrupt flag is set
        TIM6->SR &= ~TIM_SR_UIF; // Clear the interrupt flag
        
        counter++; 
				char t[100];
				//sprintf(t, "%d", counter);
	
				//UART_SendString(USART2,t);
        if ((counter % traffic_monitor) == 0) {

						print_monitor();
        }
    }
}
// Function to initialize Timer 6
void TIM6_Config(void) {
    RCC->APB1ENR |= TIM6_CLOCK; 
    
    TIM6->PSC = 8999; //10khz frequency ....
    TIM6->ARR = 9999; 
    
    TIM6->DIER |= TIM_DIER_UIE; 
    NVIC_EnableIRQ(TIM6_DAC_IRQn); 
    
    TIM6->CR1 |= TIM_CR1_CEN; 
}
/*
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
}*/
