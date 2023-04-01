#include "stm32f446xx.h"
#include "stm32f4xx.h"
#include "global.h"
#include "CLOCK.h"
#include "USART.h"
#include "GPIO.h"
#include "SYS_INIT.h"

void USART2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART6_IRQHandler(void);
void light_to_control(char* );
void control_to_light(char* );
void write_to_inp_buffer(char* );
uint8_t parse_string(char* , int* );