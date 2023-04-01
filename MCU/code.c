#include "CLOCK.h"
#include "USART.h"
#include "GPIO.h"
#include "SYS_INIT.h"
#include "InterruptHandler.h"
#include "Timer.h"
#include "global.h"
#include <stdio.h>
#include <string.h>

#include <stdlib.h>


//vertical GPIOA
//horizontal GPIOB



//GPIOA
#define UD_red 5
#define UD_yellow 6
#define UD_green 7


//GPIOA
#define U_1 13 
#define U_2 14
#define U_3 15

//GPIOA 
#define D_1 10
#define D_2 11
#define D_3 12

//GPIOB
#define LR_red 3
#define LR_yellow 4
#define LR_green 5

//GPIOB
#define R_1 13
#define R_2 14
#define R_3 15 //2

#define L_1 10
#define L_2 11 //1
#define L_3 12



#define UDMOVE GPIOA
#define LRMOVE GPIOB


uint16_t trafficU=0,trafficD=0,trafficL=0,trafficR=0; //traffic count
uint16_t UD=0,LR=0; //direction flag


//int32_t Red_to_Green=10; //actual 60
//int32_t Green_to_Yellow=5; //actual 5
//int32_t Yellow_to_Red=2; //actual 2
//int32_t extra_time=5; //actual 

int32_t UD_Red_to_Green; //actual 60
int32_t UD_Green_to_Yellow; //actual 5
int32_t UD_Yellow_to_Red; //actual 2
int32_t UD_extra_time; //actual 15

int32_t LR_Red_to_Green; //actual 60
int32_t LR_Green_to_Yellow; //actual 5
int32_t LR_Yellow_to_Red; //actual 2
int32_t LR_extra_time; //actual 15 */


static GPIO_InitTypeDef gpio_LRlight;
static GPIO_InitTypeDef gpio_UDlight;
void traffic_cnt(GPIO_TypeDef* GPIOx,uint16_t x,uint16_t y);
int pin_status(GPIO_TypeDef* GPIOx,uint16_t pin);
int get_value(uint16_t pin);

void update_signal()
{
	if(get_value(9)) //checking if LR red is on
	{
				UD=1;
				LR=0;
	}
	else 
	{
				LR=1;
				UD=0;
	}
}

void reset_led(GPIO_TypeDef* GPIOx)	
{
	for(uint16_t i=10; i<=15; i++)
	{
		if(GPIOx==GPIOB && i==11) GPIO_WritePin(GPIOx,1,GPIO_PIN_RESET);
		else if(GPIOx==GPIOB && i==15) GPIO_WritePin(GPIOx,2,GPIO_PIN_RESET);
		else GPIO_WritePin(GPIOx,i,GPIO_PIN_RESET);
	}
}
void traffic_indicator()
{
		if(LR==1)
		{
			reset_led(GPIOA);
			traffic_cnt(GPIOA,trafficD,trafficU);
		}
		else if(UD==1)
		{
			reset_led(GPIOB);
			traffic_cnt(GPIOB,trafficL,trafficR);
		}
}
void traffic_move(int n,GPIO_TypeDef* GPIOx)
{
	uint16_t st1=12;
	uint16_t st2=15;
	
	
	for(int i=0; i<n; i++)
	{
		
		reset_led(GPIOx);
		/*if(get_value(12))
		{
			UART_SendString(USART2,"ON ase");
		}*/
		if(con_flag==1)
		{
					con_flag=0;
					control_to_light(con_buffer_in);
					strcpy(con_buffer_in,"");
					
		}
		else if(tim_flag==1)
		{
			tim_flag=0;
			UART_SendString(USART2,"-----------------------------------");
			light_to_control(sen);
			UART_SendString(USART2,"-----------------------------------");
		}
		
		else {
			ms_delay(250);
		}

		for(uint16_t j=0; j<3; j++)
		{
			if(j>0)
			{
				
				if(GPIOx==GPIOB && st1-j+1==11) GPIO_WritePin(GPIOx,1,GPIO_PIN_RESET);
				else GPIO_WritePin(GPIOx,st1-j+1,GPIO_PIN_RESET);
				if(GPIOx==GPIOB && st2-j+1==15) GPIO_WritePin(GPIOx,2,GPIO_PIN_RESET);
				else GPIO_WritePin(GPIOx,st2-j+1,GPIO_PIN_RESET);
			}
			if(GPIOx==GPIOB && st1-j==11) GPIO_WritePin(GPIOx,1,GPIO_PIN_SET);	
			else GPIO_WritePin(GPIOx,st1-j,GPIO_PIN_SET);
			if(GPIOx==GPIOB && st2-j==15) GPIO_WritePin(GPIOx,2,GPIO_PIN_SET);
			else GPIO_WritePin(GPIOx,st2-j,GPIO_PIN_SET);
			ms_delay(250);
			
		}
	
	} 
	reset_led(GPIOx);
}
void traffic_LR_go()
{
	trafficL=0;
	trafficR=0;
	GPIO_WritePin(GPIOB,LR_red,GPIO_PIN_RESET);
	GPIO_WritePin(GPIOB,LR_green,GPIO_PIN_SET);
	update_signal();
	traffic_indicator();
	traffic_move(LR_Red_to_Green,GPIOB);

}
void traffic_LR_stop()
{
		GPIO_WritePin(GPIOB,LR_green,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB,LR_yellow,GPIO_PIN_SET);
		traffic_move(LR_Green_to_Yellow,GPIOB);
		GPIO_WritePin(GPIOB,LR_yellow,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOB,LR_red,GPIO_PIN_SET);
		update_signal();

}
void traffic_UD_go()
{
		
		trafficU=0;
		trafficD=0;
		GPIO_WritePin(GPIOA,UD_red,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA,UD_green,GPIO_PIN_SET);
		update_signal();
		traffic_indicator();
		traffic_move(UD_Red_to_Green,GPIOA);

}

void traffic_UD_stop()
{
		GPIO_WritePin(GPIOA,UD_green,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA,UD_yellow,GPIO_PIN_SET);
		traffic_move(UD_Green_to_Yellow,GPIOA);
		GPIO_WritePin(GPIOA,UD_yellow,GPIO_PIN_RESET);
		GPIO_WritePin(GPIOA,UD_red,GPIO_PIN_SET);
		update_signal();

}
void traffic_cnt(GPIO_TypeDef* GPIOx,uint16_t x,uint16_t y)
{
	for(uint16_t i=0; i<x; i++) 
	{
		uint16_t tt=10+i;
		if(GPIOx==GPIOB && tt== (uint16_t)11) GPIO_WritePin(GPIOB,1,GPIO_PIN_SET);
		else GPIO_WritePin(GPIOx,tt,GPIO_PIN_SET);
	}
	for(uint16_t i=0; i<y; i++) 
	{
		uint16_t tt=13+i;
		if(GPIOx==GPIOB && tt== (uint16_t)15) GPIO_WritePin(GPIOB,2,GPIO_PIN_SET);
		else GPIO_WritePin(GPIOx,tt,GPIO_PIN_SET);
	}
	
}

void traffic_update()
{
		trafficD=(uint16_t)rand() % 4;
		trafficR=(uint16_t)rand() % 4;
	 	trafficU=(uint16_t)rand() % 4;
		trafficL=(uint16_t)rand() % 4;
}
int pin_status(GPIO_TypeDef* GPIOx,uint16_t pin)
{
	return(GPIOx->IDR & (1<<pin));
}
int get_value(uint16_t pin)
{
	if(pin_status(GPIOC,pin)) return 1;
	else return 0;
}




//________________________________________USART__________________________________________
char in_buffer[500];
char out_buffer[500];
char con_buffer_in[500];
char con_buffer_outp[500];
int in;
int out;
int con_in;
int con_out;
int con_flag;
int tim_flag;
int traffic_monitor;
//sending from 6 to 1


int main()
{
		con_in=0;
		con_out=00;
		con_flag=0;
		tim_flag=0;
		//usart1 for light, usart6 for control
		initClock();
		sysInit();
		UART2_Config();
		UART1_Config();
		UART6_Config();
		TIM6_Config();
	
		srand(55);
	
			traffic_monitor=20;
			UD_Red_to_Green=10; //actual 60
			UD_Green_to_Yellow=5; //actual 5
			UD_Yellow_to_Red=2; //actual 2
			UD_extra_time=5; //actual 15

			LR_Red_to_Green=10; //actual 60
		 LR_Green_to_Yellow=5; //actual 5
		 LR_Yellow_to_Red=2; //actual 2
		 LR_extra_time=5; //actual 15
		//GPIOA 5 for red, 6 yellow, 7 green for vertical
		gpio_UDlight.Pin=GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
	
		gpio_UDlight.Mode=GPIO_MODE_OUTPUT_PP;
		gpio_UDlight.Speed=GPIO_SPEED_FREQ_LOW;
		gpio_UDlight.Pull=GPIO_NOPULL;
		
		GPIO_Init(GPIOA,&gpio_UDlight);
	
		GPIOB->MODER &= ~(1UL << 13);
		GPIOC->MODER &= ~(1UL << 15);
		GPIOC->MODER &= ~(1UL << 13);
		GPIOB->MODER &= ~(1UL << 15);
		
		//GPIOB 13 for red, 14 yellow, 15 green for vertical
		gpio_LRlight.Pin=GPIO_PIN_3 | GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_2 |  GPIO_PIN_10 | GPIO_PIN_1 | GPIO_PIN_12 ;
	
		gpio_LRlight.Mode=GPIO_MODE_OUTPUT_PP;
		gpio_LRlight.Speed=GPIO_SPEED_FREQ_LOW;
		gpio_LRlight.Pull=GPIO_NOPULL;
		
		GPIO_Init(GPIOB,&gpio_LRlight);
		
		GPIO_InitTypeDef A;
		A.Pin=GPIO_PIN_8 | GPIO_PIN_4 | GPIO_PIN_1 | GPIO_PIN_5 | GPIO_PIN_9 | GPIO_PIN_10  | GPIO_PIN_11 |  GPIO_PIN_12;
		A.Mode=GPIO_MODE_INPUT;
		A.Speed=GPIO_SPEED_FREQ_LOW;
		A.Pull=GPIO_NOPULL;
		
		GPIO_Init(GPIOC,&A);
		
		ms_delay(2000);
		traffic_update();
		GPIO_WritePin(GPIOA,UD_red,GPIO_PIN_SET);
		traffic_LR_go();
		while(1)
		{
			//UART_SendString(USART2,"hi there");

			//gpioc input D 4, U 8,L 5.R 1, LR_RED 9,LR green 10,UD_green 11,UD_red 12
			traffic_update();
		  int data_D=get_value(4), data_U=get_value(8);
			int data_L=get_value(5), data_R=get_value(1);
			
			update_signal(); // updating lr and ud
		//	monitor();

			int et=0;
			if(data_U==0 && data_D==0 && LR==1) 
			{
				//UART_SendString(USART2,"UD0");
				et=1;
			}
			else if(data_L==0 && data_R==0 && UD==1 ) 
			{
				//UART_SendString(USART2,"LR0");
				et=1;
			}
			
			if(et)
			{
				if(LR==1) traffic_move(LR_extra_time,GPIOB);
				else if(UD==1) traffic_move(UD_extra_time,GPIOA);
			}
			
			if(LR==1)
			{
				

				traffic_LR_stop();
				traffic_UD_go();

			}
			else 
			{
				traffic_UD_stop();
				traffic_LR_go();
			}

		}
		
		
		/*while(1)
		{
				//UART_SendChar(USART2,cc);

			//	USART6->CR1 |= USART_CR1_TXEIE;
				//UART_SendString(USART2,"Hoyna\n");
			//	UART_SendChar(USART2,cc);
				out=0;
				in=0;
				char ab[100]="config traffic light 1 R G Y 2 10 2 2jkl\n\0";
				char cd[100]="sadasdasd\n\0";
 
			
				//light_to_control(cd);
				UART2_SendString(cd); 
				strcpy(con_buffer_outp,"");
				//UART_SendString(USART2,out_buffer); 

				ms_delay(2000);

				//control_to_light(ab);
				UART2_SendString(ab);
				strcpy(con_buffer_outp,"");


				//UART_SendString(USART2,out_buffer); 

				ms_delay(2000);

				if(con_flag==1)
				{
					con_flag=0;
					UART2_SendString(con_buffer_in);
					strcpy(con_buffer_in,"");
					
				}


		}*/
		
}