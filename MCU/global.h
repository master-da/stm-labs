#include<stdio.h>
#include<stdlib.h>
#include "stm32f446xx.h"
#include "stm32f4xx.h"
#include "USART.h"

extern int32_t UD_Red_to_Green; //actual 60
extern int32_t UD_Green_to_Yellow; //actual 5
extern int32_t UD_Yellow_to_Red; //actual 2
extern int32_t UD_extra_time; //actual 15

extern int32_t LR_Red_to_Green; //actual 60
extern int32_t LR_Green_to_Yellow; //actual 5
extern int32_t LR_Yellow_to_Red; //actual 2
extern int32_t LR_extra_time; //actual 15 

extern char in_buffer[];
extern char out_buffer[];
extern char con_buffer_in[];
extern char con_buffer_outp[];
extern char sen[];

extern  int in;
extern  int out;
extern int con_in;
extern int con_out;
extern int con_flag;
extern int tim_flag;
extern int traffic_monitor;
