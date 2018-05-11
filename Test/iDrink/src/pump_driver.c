/*
 * pump_driver.c
 *
 *  Created on: 29.04.2018
 *      Author: Bartek
 */

#include "pump_driver.h"

GPIO_TypeDef* PUMP_GPIO = GPIOE;

const uint16_t LEMON_PIN = GPIO_Pin_0;
const uint16_t ORANGE_PIN = GPIO_Pin_1;
const uint16_t RUM_PIN = GPIO_Pin_2;
const uint16_t VODKA_PIN = GPIO_Pin_3;

TIM_TypeDef* LEMON_TIMER = TIM2;
TIM_TypeDef* ORANGE_TIMER = TIM3;
TIM_TypeDef* RUM_TIMER = TIM4;
TIM_TypeDef* VODKA_TIMER = TIM5;

const uint8_t LEMON_IRQ = TIM2_IRQn;
const uint8_t ORANGE_IRQ = TIM3_IRQn;
const uint8_t RUM_IRQ = TIM4_IRQn;
const uint8_t VODKA_IRQ = TIM5_IRQn;

void lemon_IRQHandler();
void orange_IRQHandler();
void rum_IRQHandler();
void vodka_IRQHandler();

void TIM2_IRQHandler(){lemon_IRQHandler();}
void TIM3_IRQHandler(){orange_IRQHandler();}
void TIM4_IRQHandler(){rum_IRQHandler();}
void TIM5_IRQHandler(){vodka_IRQHandler();}

const int PERIOD = 9999;
const int PRESCALER = 2748;

int max_lemon_cl = 0;
int max_orange_cl = 0;
int max_rum_cl = 0;
int max_vodka_cl = 0;

int current_lemon_cl = 0;
int current_orange_cl = 0;
int current_rum_cl = 0;
int current_vodka_cl = 0;

enum bool is_lemon_pumping = false;
enum bool is_orange_pumping = false;
enum bool is_rum_pumping = false;
enum bool is_vodka_pumping = false;

void init_pump_driver()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);

	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannel = LEMON_IRQ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x01;
	NVIC_InitStructure.NVIC_IRQChannel = ORANGE_IRQ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;
	NVIC_InitStructure.NVIC_IRQChannel = RUM_IRQ;
	NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;
	NVIC_InitStructure.NVIC_IRQChannel = VODKA_IRQ;
	NVIC_Init(&NVIC_InitStructure);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = PERIOD;
	TIM_TimeBaseStructure.TIM_Prescaler = PRESCALER;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(LEMON_TIMER, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(ORANGE_TIMER, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(RUM_TIMER, &TIM_TimeBaseStructure);
	TIM_TimeBaseInit(VODKA_TIMER, &TIM_TimeBaseStructure);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = LEMON_PIN | ORANGE_PIN | RUM_PIN | VODKA_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(PUMP_GPIO, &GPIO_InitStructure);

	GPIO_SetBits(PUMP_GPIO, LEMON_PIN | ORANGE_PIN | RUM_PIN | VODKA_PIN);

	TIM_ClearITPendingBit(LEMON_TIMER, TIM_IT_Update);
	TIM_ITConfig(LEMON_TIMER, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(ORANGE_TIMER, TIM_IT_Update);
	TIM_ITConfig(ORANGE_TIMER, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(LEMON_TIMER, TIM_IT_Update);
	TIM_ITConfig(RUM_TIMER, TIM_IT_Update, ENABLE);

	TIM_ClearITPendingBit(LEMON_TIMER, TIM_IT_Update);
	TIM_ITConfig(VODKA_TIMER, TIM_IT_Update, ENABLE);
}

void pump(enum liquid liq, int amount_cl)
{
	switch (liq) {
		case lemon:
			if(is_lemon_pumping == false){
				max_lemon_cl = amount_cl;
				current_lemon_cl = 0;
				start_lemon_pump();
			}
			break;
		case orange:
			if(is_orange_pumping == false){
				max_orange_cl = amount_cl;
				current_orange_cl = 0;
				start_orange_pump();
			}
			break;
		case rum:
			if(is_rum_pumping == false){
				max_rum_cl = amount_cl;
				current_rum_cl = 0;
				start_rum_pump();
			}
			break;
		case vodka:
			if(is_vodka_pumping == false){
				max_vodka_cl = amount_cl;
				current_vodka_cl = 0;
				start_vodka_pump();
			}
			break;
		default:
			break;
	}
}

void start_lemon_pump()
{
	is_lemon_pumping = true;
	TIM_Cmd(LEMON_TIMER,ENABLE);
	GPIO_ResetBits(GPIOE,LEMON_PIN);
}

void start_orange_pump()
{
	is_orange_pumping = true;
	TIM_Cmd(ORANGE_TIMER,ENABLE);
	GPIO_ResetBits(GPIOE,ORANGE_PIN);
}

void start_rum_pump()
{
	is_rum_pumping = true;
	TIM_Cmd(RUM_TIMER,ENABLE);
	GPIO_ResetBits(GPIOE,RUM_PIN);
}

void start_vodka_pump()
{
	is_vodka_pumping = true;
	TIM_Cmd(VODKA_TIMER, ENABLE);
	GPIO_ResetBits(GPIOE,VODKA_PIN);
}

void lemon_IRQHandler()
{
	if (TIM_GetITStatus(LEMON_TIMER, TIM_IT_Update) != RESET) {
		if (current_lemon_cl < max_lemon_cl)
		{
			current_lemon_cl++;
		}
		else
		{
			GPIO_SetBits(PUMP_GPIO, LEMON_PIN);
			TIM_Cmd(LEMON_TIMER, DISABLE);
			max_lemon_cl = 0;
			current_lemon_cl = 0;
			is_lemon_pumping = false;
		}
		TIM_ClearITPendingBit(LEMON_TIMER, TIM_IT_Update);
	}
}

void orange_IRQHandler()
{
	if (TIM_GetITStatus(ORANGE_TIMER, TIM_IT_Update) != RESET) {
		if (current_orange_cl < max_orange_cl)
		{
			current_orange_cl++;
		}
		else
		{
			GPIO_SetBits(PUMP_GPIO, ORANGE_PIN);
			TIM_Cmd(ORANGE_TIMER, DISABLE);
			max_orange_cl = 0;
			current_orange_cl = 0;
			is_orange_pumping = false;
		}
		TIM_ClearITPendingBit(ORANGE_TIMER, TIM_IT_Update);
	}
}

void rum_IRQHandler()
{
	if (TIM_GetITStatus(RUM_TIMER, TIM_IT_Update) != RESET) {
		if (current_rum_cl < max_rum_cl)
		{
			current_rum_cl++;
		}
		else
		{
			GPIO_SetBits(PUMP_GPIO, RUM_PIN);
			TIM_Cmd(RUM_TIMER, DISABLE);
			max_rum_cl = 0;
			current_rum_cl = 0;
			is_rum_pumping = false;
		}
		TIM_ClearITPendingBit(RUM_TIMER, TIM_IT_Update);
	}
}

void vodka_IRQHandler()
{
	if (TIM_GetITStatus(VODKA_TIMER, TIM_IT_Update) != RESET) {
		if (current_vodka_cl < max_vodka_cl)
		{
			current_vodka_cl++;
		}
		else
		{
			GPIO_SetBits(PUMP_GPIO, VODKA_PIN);
			TIM_Cmd(VODKA_TIMER, DISABLE);
			max_vodka_cl = 0;
			current_vodka_cl = 0;
			is_vodka_pumping = false;
		}
		TIM_ClearITPendingBit(VODKA_TIMER, TIM_IT_Update);
	}
}

