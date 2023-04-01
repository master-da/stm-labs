#include "InterruptHandler.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "global.h"
#include <stdlib.h>


void configuration();

uint8_t parse_string(char* str, int* values) {
    uint8_t expected_num_values = 0;

    // Check for "config traffic light x G Y R w y z u" format
    if (sscanf(str, "config traffic light %d %*s %*s %*s %d %d %d %d", &values[0], &values[1], &values[2], &values[3],&values[4]) == 5) {
        expected_num_values=5;
        if (values != NULL) {
            sscanf(str, "config traffic light %d %*s %*s %*s %d %d %d %d", &values[0], &values[1], &values[2], &values[3],&values[4]);
        }
    }
    // Check for "config traffic monitor X" format
    else if (sscanf(str, "config traffic monitor %d", &values[0]) == 1) {
        expected_num_values=1;

        if (values != NULL) {
            sscanf(str, "config traffic monitor %d", &values[0]);
        }
    }
    // Check for "read traffic light x" format
    else if (sscanf(str, "read traffic light %d", &values[0]) == 1) {
        expected_num_values=1;

        if (values != NULL) {
            sscanf(str, "read traffic light %d", &values[0]);
        }
    }
		 

    return expected_num_values;
}

void USART2_IRQHandler(void) {
	
	if (USART2->SR & USART_SR_RXNE) 
	{
		while(!(USART2->SR & USART_SR_RXNE));
		char c=(uint8_t)USART2->DR;
		con_buffer_in[con_in++]= c;
		USART2->SR &= ~USART_SR_RXNE;
		
		if(c=='\0' || c=='\n')
		{
			con_buffer_in[con_in]= '\0';
			con_flag=1;
			con_in=0;
			//UART_SendString(USART2,con_buffer_in);
		}
		
	}
	if (USART2->SR & USART_SR_TXE){
		
        USART2->DR =	con_buffer_outp[con_out];
        while(!(USART2->SR & USART_SR_TXE));
		
        USART2->SR &= ~(USART_SR_TXE);
        USART2->CR1 &= ~(USART_CR1_TXEIE);
    }
}
/*
void USART2_IRQHandler(void){
	
	uint8_t da[50]; 
	
	if (USART2->SR & USART_SR_RXNE) {
		//USART2->CR1 &= ~USART_CR1_RXNEIE;
		UART_GetString(USART2, da);
		int values[10] = {0}; // Initialize values array with zeros
		uint8_t a = parse_string(out_buffer, values);
    if(a==5)
		{

			UD_Green_to_Yellow=values[2];
			UART_SendString(USART2, "hola amigos");

		}
	//	USART2->CR1 |= USART_CR1_RXNEIE;
		
		USART2->SR &= ~USART_SR_RXNE_Msk;
	}

}*/

void USART6_IRQHandler(void) {
	
	if (USART6->SR & USART_SR_RXNE) 
	{
		while(!(USART6->SR & USART_SR_RXNE));
		out_buffer[out]= (uint8_t)USART6->DR;
		USART6->SR &= ~USART_SR_RXNE;
	}
	if (USART6->SR & USART_SR_TXE){
        USART6->DR =	in_buffer[in];
        while(!(USART6->SR & USART_SR_TXE));
        USART6->SR &= ~(USART_SR_TXE);
        USART6->CR1 &= ~(USART_CR1_TXEIE);
    }
}
void USART1_IRQHandler(void) {
	
	if (USART1->SR & USART_SR_RXNE) 
	{
		while(!(USART1->SR & USART_SR_RXNE));
		out_buffer[out] = (uint8_t) USART1->DR;
		USART1->SR &= ~USART_SR_RXNE;
		
	}
	if (USART1->SR & USART_SR_TXE){
        USART1->DR = 	in_buffer[in];
        
        while(!(USART1->SR & USART_SR_TXE));
        USART1->SR &= ~(USART_SR_TXE);
        USART1->CR1 &= ~(USART_CR1_TXEIE);
    }
}

char xra[20];
void int_to_str(char *s,int x)
{
	sprintf(xra, "%d", x);
	xra[strlen(xra)]=' ';
	strcat(s,xra);
	
}
void	create_read1(char *tmp1)
{
			int_to_str(tmp1,UD_Red_to_Green);
			int_to_str(tmp1,UD_Green_to_Yellow);
			int_to_str(tmp1,UD_Yellow_to_Red);
			int_to_str(tmp1,UD_extra_time);	

}
void	create_read2(char *tmp1)
{
			int_to_str(tmp1,LR_Red_to_Green);
			int_to_str(tmp1,LR_Green_to_Yellow);
			int_to_str(tmp1,LR_Yellow_to_Red);
			int_to_str(tmp1,LR_extra_time);	

}


void write_to_inp_buffer(char* s)
{
	strcpy(in_buffer,"");
	strcpy(out_buffer,"");
	//UART2_SendString(s);

	int i=0,j=0;
	for( i=0; i<strlen(s); i++)
	{
		in_buffer[i]=s[i];
	}
	in_buffer[i]='\0';
	//UART2_SendString(in_buffer);


}
//light USART6 , Control USART1
void light_to_control(char* s)
{
	write_to_inp_buffer(s);

	in=0;
	out=0;
	GPIOC->MODER |= (2<<12);
	GPIOB->MODER |= (2<<14);
	for(int i=0; i<strlen(in_buffer); i++)
	{
		USART6->CR1 |= USART_CR1_TXEIE;
		//while(USART6->CR1 & USART_CR1_TXEIE);
		ms_delay(2);
		in++;
		out++;
	}
	GPIOC->MODER &= ~(1UL << 13);
	GPIOB->MODER &= ~(1UL << 15);
	UART_SendString(USART2, out_buffer); //control center print
	strcpy(in_buffer,"");

}

void control_to_light(char* s)
{
	GPIOB->MODER |= (2<<12);
	GPIOC->MODER |= (2<<14);
	write_to_inp_buffer(s);
	
	in=0;
	out=0;
	
	for(int i=0; i<strlen(in_buffer); i++)
	{
		USART1->CR1 |= USART_CR1_TXEIE;
		//while(USART6->CR1 & USART_CR1_TXEIE);
		ms_delay(2);
		in++;
		out++;
		
	}
	GPIOB->MODER &= ~(1UL << 13);
	GPIOC->MODER &= ~(1UL << 15);
	strcpy(in_buffer,"");

		configuration(); //eta on krte hbe
	
}
void configuration()
{
	int values[10] = {0}; // Initialize values array with zeros
	uint8_t a = parse_string(out_buffer, values);
	if(a==0 && out_buffer[0]=='r')
		{					
			
				
			//UART_SendString(USART2,out_buffer);
							if(strlen(con_buffer_in)>8)
							{
								char tmp3[50]="\ntraffic monitor ";
								int_to_str(tmp3,traffic_monitor);
								light_to_control(tmp3);
							}
							else
							{
									char tmp1[100]="\ntraffic light 1 G Y R ";
									char tmp2[100]="\ntraffic light 2 G Y R ";
									char tmp3[50]="\ntraffic monitor ";
									create_read1(tmp1);
									create_read2(tmp2);	
									int_to_str(tmp3,traffic_monitor);
									strcat(tmp1,tmp2);
									strcat(tmp1,tmp3);
									light_to_control(tmp1);
							}
							
						
		}
		else if((a==1) && out_buffer[0]=='r')
		{
			
			if (sscanf(out_buffer, "read traffic light %d", &values[0]) == 1) 
			{
					if(values[0]==1)
					{
							char tmp1[100]="\ntraffic light 1 G Y R ";
							create_read1(tmp1);
							light_to_control(tmp1);
					}
					else if(values[0]==2)
					{
							char tmp2[100]="\ntraffic light 2 G Y R ";
							create_read2(tmp2);
							light_to_control(tmp2);
					}
			}
			
		}
		else if((a==1) && out_buffer[0]=='c')
		{
			traffic_monitor=values[0];
		}

    else if(a==5)
		{
			if(values[0]==1)
			{

				UD_Red_to_Green=values[1];
				UD_Green_to_Yellow=values[2];
				LR_Red_to_Green=values[3];
				UD_Yellow_to_Red=values[3];
				UD_extra_time=values[4];
				//UART_SendString(USART2, "1 configured");
				
			}
			else if(values[0]==2)
			{
				LR_Red_to_Green=values[1];
				LR_Green_to_Yellow=values[2];
				UD_Red_to_Green=values[3];
				LR_Yellow_to_Red=values[3];
				LR_extra_time=values[4];
				//UART_SendString(USART2, "2 configured");
			}
			
			
		}
}
