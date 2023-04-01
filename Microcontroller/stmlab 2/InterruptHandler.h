#include <string.h>
#include "stm32f446xx.h"
#include "stm32f4xx.h"
#include "USART.h"
#include "Timer.h"
#include "StringParser.h"

void USART2_IRQHandler(void);
void USART1_IRQHandler(void);
void USART6_IRQHandler(void);
