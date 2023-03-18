#include "stm32f446xx.h"
#include "stm32f4xx.h"
#include "SYS_INIT.h"
#include "CLOCK.h"
#include "GPIO.h"
#include "Timer.h"
#include "USART.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define TIME_RED_TO_GREEN 5 /* Remember to change back to 60 */
#define TIME_GREEN_TO_YELLOW 5
#define TIME_YELLOW_TO_RED 2

static uint8_t traffic_cars_1[] = {9, 6, 8}; // B
static uint8_t traffic_cars_2[] = {9, 8, 10}; // A
static uint8_t traffic_cars_3[] = {5,  12, 11}; // A
static uint8_t traffic_cars_4[] = {5, 4, 10}; // B
static uint8_t traffic_lights_1[] = {8, 6, 5}; // C, 8 -> GREEN, 6 -> RED, 5 -> YELLOW
static uint8_t traffic_lights_2[] = {10, 11, 12}; // C, 10 -> RED, 11 -> GREEN, 12 -> YELLOW

static uint8_t a_pins[] = {8, 9, 10, 11, 12, 5};
static uint8_t b_pins[] = {4, 5, 10, 8, 9, 6};
static uint8_t c_pins[] = {8, 6, 5, 10, 11, 12};

static uint8_t c_pins_input[] = {0, 1, 2, 3};

static uint16_t lst = 0;

void init_gpio_pins(void);
void enable_clock(void);
void init_input_pins(void);

void handle_up_down(uint16_t load);
void handle_down_up(uint16_t load);
void handle_left_right(uint16_t load);
void handle_right_left(uint16_t load);

void handle_inputs(void);

void clearLEDs(void);
uint16_t get_rand(int16_t l, int16_t r);

int main(void) {
	
	uint16_t up_down_load;
	uint16_t down_up_load;
	uint16_t left_right_load;
	uint16_t right_left_load;
	
	srand(69);
	initClock();
	sysInit();
	TIM6_Config();
	//UART1_Config();
	UART2_Config();
	//UART4_Config();

	//enable_clock();	// enables the clocks for GPIO A, B, C
	//init_gpio_pins();	// enalbles the pins described in a_pins, b_pins and c_pins
		
	while(1){
		USART2->DR = 'A';
		while (!(USART2->SR & 1 << 7));
		
		delay_ms(TIM6, 1000);
	} // to stop the lights from blinding me
	
	// order -> 2, 4, 1, 3
	up_down_load = get_rand(0, 3);
	handle_up_down(up_down_load);
	clearLEDs();
	
	while (1) {
		uint16_t cur = get_rand(0, 3);
		
		while (cur == lst) cur = get_rand(0, 3);
		
		up_down_load = get_rand(0, 3);
		down_up_load = get_rand(0, 3);
		left_right_load = get_rand(0, 3);
		right_left_load = get_rand(0, 3);
		
		switch (cur) {
			case 0:
				handle_up_down(up_down_load);
				break;
			case 1:
				handle_down_up(down_up_load);
				break;
			case 2:
				handle_left_right(left_right_load);
				break;
			case 3:
				handle_right_left(right_left_load);
				break;
			default:
				assert(0);
		}
		
		clearLEDs();
		ms_delay(1000);
		lst = cur;
	}
}

void handle_up_down(uint16_t load) {
	
	uint32_t extra_time;
	uint16_t div1;
	uint16_t div2;
	
	if (!load) return;
	GPIO_WritePin(GPIOC, 6, GPIO_PIN_SET);
	extra_time = (load > 2 ? 2 : 0);
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOA, traffic_cars_2[i], GPIO_PIN_SET);
	ms_delay((TIME_RED_TO_GREEN + extra_time) * 1000 / 2);
	div1 = get_rand(0, (int16_t) load - 1);
	div2 = load - div1;
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOA, traffic_cars_2[i], GPIO_PIN_RESET);
	for (int i = 0; i < div1; ++i) GPIO_WritePin(GPIOB, traffic_cars_1[i], GPIO_PIN_SET);
	ms_delay((TIME_RED_TO_GREEN + extra_time) * 1000 / 2);
	handle_inputs();
	GPIO_WritePin(GPIOC, 6, GPIO_PIN_RESET);
	GPIO_WritePin(GPIOC, 5, GPIO_PIN_SET);
	for (int i = 0; i < div2; ++i) GPIO_WritePin(GPIOB, traffic_cars_4[i], GPIO_PIN_SET);
	ms_delay(TIME_YELLOW_TO_RED * 1000);
}

void handle_down_up(uint16_t load) {
	
	uint32_t extra_time;
	uint16_t div1;
	uint16_t div2;
	
	if (!load) return;
	GPIO_WritePin(GPIOC, 8, GPIO_PIN_SET);
	extra_time = (load > 2 ? 2 : 0);
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOB, traffic_cars_4[i], GPIO_PIN_SET);
	ms_delay((TIME_GREEN_TO_YELLOW + extra_time) * 1000 / 2);
	div1 = get_rand(0, (int16_t) load - 1);
	div2 = load - div1;
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOB, traffic_cars_4[i], GPIO_PIN_RESET);
	for (int i = 0; i < div1; ++i) GPIO_WritePin(GPIOA, traffic_cars_3[i], GPIO_PIN_SET);
	ms_delay((TIME_GREEN_TO_YELLOW + extra_time) * 1000 / 2);
	handle_inputs();
	GPIO_WritePin(GPIOC, 8, GPIO_PIN_RESET);
	GPIO_WritePin(GPIOC, 5, GPIO_PIN_SET);
	for (int i = 0; i < div2; ++i) GPIO_WritePin(GPIOA, traffic_cars_2[i], GPIO_PIN_SET);
	ms_delay(TIME_YELLOW_TO_RED * 1000);
}

void handle_left_right(uint16_t load) {
	
	uint32_t extra_time;
	uint16_t div1;
	uint16_t div2;
	
	if (!load) return;
	GPIO_WritePin(GPIOC, 11, GPIO_PIN_SET);
	extra_time = (load > 2 ? 2 : 0);
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOB, traffic_cars_1[i], GPIO_PIN_SET);
	ms_delay((TIME_GREEN_TO_YELLOW + extra_time) * 1000 / 2);
	div1 = get_rand(0, (int16_t) load - 1);
	div2 = load - div1;
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOB, traffic_cars_1[i], GPIO_PIN_RESET);
	for (int i = 0; i < div1; ++i) GPIO_WritePin(GPIOA, traffic_cars_3[i], GPIO_PIN_SET);
	ms_delay((TIME_GREEN_TO_YELLOW + extra_time) * 1000 / 2);
	handle_inputs();
	GPIO_WritePin(GPIOC, 11, GPIO_PIN_RESET);
	GPIO_WritePin(GPIOC, 12, GPIO_PIN_SET);
	for (int i = 0; i < div2; ++i) GPIO_WritePin(GPIOB, traffic_cars_4[i], GPIO_PIN_SET);
	ms_delay(TIME_YELLOW_TO_RED * 1000);
}

void handle_right_left(uint16_t load) {
	
	uint32_t extra_time;
	uint16_t div1;
	uint16_t div2;
	
	if (!load) return;
	GPIO_WritePin(GPIOC, 10, GPIO_PIN_SET);
	extra_time = (load > 2 ? 2 : 0);
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOA, traffic_cars_3[i], GPIO_PIN_SET);
	ms_delay((TIME_RED_TO_GREEN + extra_time) * 1000 / 2);
	div1 = get_rand(0, (int16_t) load - 1);
	div2 = load - div1;
	for (int i = 0; i < load; ++i) GPIO_WritePin(GPIOA, traffic_cars_3[i], GPIO_PIN_RESET);
	for (int i = 0; i < div1; ++i) GPIO_WritePin(GPIOB, traffic_cars_1[i], GPIO_PIN_SET);
	ms_delay((TIME_RED_TO_GREEN + extra_time) * 1000 / 2);
	handle_inputs();
	GPIO_WritePin(GPIOC, 10, GPIO_PIN_RESET);
	GPIO_WritePin(GPIOC, 12, GPIO_PIN_SET);
	for (int i = 0; i < div2; ++i) GPIO_WritePin(GPIOA, traffic_cars_2[i], GPIO_PIN_SET);
	ms_delay(TIME_YELLOW_TO_RED * 1000);	
}

void enable_clock(void) {
	enable_clock_for_register(0U);
	enable_clock_for_register(1U);
	enable_clock_for_register(2U);
}

void init_gpio_pins(void) {
	GPIO_InitTypeDef init_type_def;
	for (int i = 0; i < 6; ++i) {
		init_type_def = get_init_type_def(a_pins[i], GPIO_MODE_OUTPUT_PP, 0U, 0U, 0U);
		GPIO_Init(GPIOA, &init_type_def);
	}
	for (int i = 0; i < 6; ++i) {
		init_type_def = get_init_type_def(b_pins[i], GPIO_MODE_OUTPUT_PP, 0U, 0U, 0U);
		GPIO_Init(GPIOB, &init_type_def);
	}
	for (int i = 0; i < 6; ++i) {
		init_type_def = get_init_type_def(c_pins[i], GPIO_MODE_OUTPUT_PP, 0U, 0U, 0U);
		GPIO_Init(GPIOC, &init_type_def);
	}
}

void init_input_pins(void) {
	GPIO_InitTypeDef init_type_def;
	for (int i = 0; i < 4; ++i) {
		init_type_def = get_init_type_def(c_pins_input[i], GPIO_MODE_INPUT, 0U, 0U, 0U);
		GPIO_Init(GPIOC, &init_type_def);
	}
}

void handle_inputs(void) {
	uint32_t line1_green = (1U << 0) & GPIOC->IDR;
	uint32_t line1_red = (1U << 1) & GPIOC->IDR;
	uint32_t line2_green = (1U << 2) & GPIOC->IDR;
	uint32_t line2_red = (1U << 3) & GPIOC->IDR;
		
	if (line1_red) lst = 0;
	if (line1_green) lst = 1;
	if (line2_green) lst = 2;
	if (line2_red) lst = 3;
}

uint16_t get_rand(int16_t l, int16_t r) { 
	assert(l <= r);
	return (uint16_t) rand() % (r - l + 1) + (uint16_t) l; 
}

void clearLEDs(void) {
	for (int i = 0; i < 3; ++i) GPIO_WritePin(GPIOB, traffic_cars_1[i], GPIO_PIN_RESET);
	for (int i = 0; i < 3; ++i) GPIO_WritePin(GPIOA, traffic_cars_2[i], GPIO_PIN_RESET);
	for (int i = 0; i < 3; ++i) GPIO_WritePin(GPIOA, traffic_cars_3[i], GPIO_PIN_RESET);
	for (int i = 0; i < 3; ++i) GPIO_WritePin(GPIOB, traffic_cars_4[i], GPIO_PIN_RESET);
	for (int i = 0; i < 3; ++i) GPIO_WritePin(GPIOC, traffic_lights_1[i], GPIO_PIN_RESET);
	for (int i = 0; i < 3; ++i) GPIO_WritePin(GPIOC, traffic_lights_2[i], GPIO_PIN_RESET);
}
