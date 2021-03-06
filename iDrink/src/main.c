/**
  ******************************************************************************
  * @file    main.c
  * @author  Ac6
  * @version V1.0
  * @date    01-December-2013
  * @brief   Default main function.
  ******************************************************************************
*/


#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_delay.h"
#include "tm_stm32f4_hd44780.h"
#include "defines.h"
#include "menu.h"
#include "pump_driver.h"

unsigned char selected = 1;

const char menu_000[] = "     [Menu]       "; //0
const char menu_001[] = "  Wybierz drink   "; //1
const char menu_002[] = "  Czyszczenie     "; //2
const char menu_003[] = "  Wykonawcy       "; //3
const char menu_004[] = "                  "; //4

const char menu_100[] = "     [SubMenu]    "; //5
const char menu_101[] = "  Bacardi         "; //6
const char menu_102[] = "  Cuba Libre      "; //7
const char menu_103[] = "  Shot of vodka   "; //8
const char menu_104[] = "  Shot of vodka J "; //9
const char menu_105[] = "  Drink (vodka+J) "; //10
const char menu_106[] = "  Drink (codka+C) "; //11
const char menu_107[] = "  Try me          "; //12
const char menu_108[] = "  Wroc            "; //13


MenuEntry menu[] =
{
		{menu_000, 5, 0, 0, 0, 0},
		{menu_001, 5, 1, 2, 6, 0},
		{menu_002, 5, 1, 3, 2, clean},
		{menu_003, 5, 2, 4, 3, info},
		{menu_004, 5, 3, 4, 4, start},

		{menu_100, 9, 0, 0, 0, 0},
		{menu_101, 9, 6, 7, 6, bacardi},
		{menu_102, 9, 6, 8, 7, cuba_libre},
		{menu_103, 9, 7, 9, 8, vodka_shot},
		{menu_104, 9, 8, 10, 9, vodka_juice},
		{menu_105, 9, 9, 11, 10, vodka_shot_juice},
		{menu_106, 9, 10, 12, 11, vodka_cola},
		{menu_107, 9, 11, 13, 12, kociolek},
		{menu_108, 9, 12, 13, 1, 0}
};

void show_menu(void){
	unsigned char line_counter = 0;
	unsigned char from = 0;
	unsigned char till = 0;
	unsigned char temp = 0;

	while(till <= selected)
	{
		till += menu[till].num_menupoints;
	}
	from = till - menu[selected].num_menupoints;
	till--;

	temp = from;

	if((selected >= (from + 2)) && (selected <= (till-1))){
		from = selected -2;
		till = from +3;
		for(from;from<=till;from++){
			TM_HD44780_Puts(0, line_counter, menu[from].text);
			line_counter++;
		}
		TM_HD44780_Puts(0, 2, "-");
		if(menu[from].num_menupoints==5)TM_HD44780_Puts(0, 0, menu[0].text);
		else TM_HD44780_Puts(0, 0, menu[5].text);
	}
	else{
		if(selected < (from +2)){
			till=from+3;
			for(from;from<=till;from++){
				TM_HD44780_Puts(0, line_counter, menu[from].text);
				line_counter++;
			}

			TM_HD44780_Puts(0, selected-temp, "-");
			if(menu[from].num_menupoints==5)TM_HD44780_Puts(0, 0, menu[0].text);
			else TM_HD44780_Puts(0, 0, menu[5].text);
		}
		if(selected == till){
			from = till-3;
			for(from;from<=till;from++){
				TM_HD44780_Puts(0, line_counter, menu[from].text);
				line_counter++;
			}
			TM_HD44780_Puts(0, 3, "-");
			if(menu[from].num_menupoints==5)TM_HD44780_Puts(0, 0, menu[0].text);
			else TM_HD44780_Puts(0, 0, menu[5].text);
		}
	}
}


void browse_menu(void){
	do{
		show_menu();
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))
		{
			selected = menu[selected].up;
		}
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2))
		{
			selected = menu[selected].down;
		}
		if(!GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1))
		{
			if(menu[selected].fp != 0)
				menu[selected].fp();
			selected = menu[selected].enter;
		}
		Delayms(150);

	}while(1);
}

void start(void){
	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "start works!");
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1));
}

void bacardi(void){
	pump(orange, 8);
	Delayms(5000);
	pump(rum, 8);
}

void cuba_libre(void){
	pump(rum, 6);
	pump(orange, 6);
	pump(lemon, 6);
}

void vodka_shot(void){
	pump(vodka, 4);
}
void vodka_juice(void){
	pump(vodka, 8);
	pump(lemon, 8);
}
void vodka_shot_juice(void){
	pump(vodka, 2);
	pump(lemon, 2);
}
void vodka_cola(void){
	pump(vodka, 8);
	pump(orange, 8);
}
void kociolek(void){
	pump(rum, 4);
	pump(orange, 5);
	pump(lemon, 5);
	pump(vodka, 3);
}
void clean(void){
	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "UWAGA");
	TM_HD44780_Puts(0, 1, "   Czyszczenie");
	TM_HD44780_Puts(0, 2, "   podloz naczynie");
	Delayms(3000);
	int p = 53;
	char pom[] = "X";
	while(p>48){
		pom[0] = p;
		TM_HD44780_Clear();
		TM_HD44780_Puts(10, 2, pom);
		Delayms(1000);
		p-=1;
	}
	pump(rum, 4);
	pump(orange, 4);
	pump(lemon, 4);
	pump(vodka, 4);
}

void info(void){
	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "Szymon Janowski");
	TM_HD44780_Puts(0, 1, "Bartosz Mikulski");
	Delayms(3000);
	TM_HD44780_Clear();
	TM_HD44780_Puts(0, 0, "PUT");
	TM_HD44780_Puts(0, 1, "Poznan 2018");
	Delayms(2000);
}

void init(){
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	init_pump_driver();

	GPIO_InitTypeDef Przyciski;
	Przyciski.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	Przyciski.GPIO_Mode = GPIO_Mode_IN;
	Przyciski.GPIO_OType = GPIO_OType_PP;
	Przyciski.GPIO_Speed = GPIO_Speed_100MHz;
	Przyciski.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &Przyciski);

//	GPIO_InitTypeDef GPIO_InitStructure;
//	GPIO_InitStructure.GPIO_Pin =
//	GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
//	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//
//	GPIO_SetBits(GPIOD, GPIO_Pin_14);
}


void USART3_IRQHandler(void)
{
    if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
    {
            if (USART3->DR == 'B' )
            {
                bacardi();
            }
            if (USART3->DR == 'C' )
            {
               cuba_libre();
            }
            if (USART3->DR == 'V' )
            {
               vodka_shot();
            }
            if (USART3->DR == 'K' )
            {
               kociolek();
            }
            if (USART3->DR == 'W' )
            {
               clean();
            }
            if (USART3->DR == 'I' )
            {
               info();
            }
    }
}


void sendData(char character){
    while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);

    USART_SendData(USART3, character);

    while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);
}

void UART(){
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);


    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART3, &USART_InitStructure);


    USART_Cmd(USART3, ENABLE);
}

void UART_IRQ(){
    NVIC_InitTypeDef NVIC_InitStructure2;

    USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
    NVIC_InitStructure2.NVIC_IRQChannel = USART3_IRQn;
    NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure2);

    NVIC_EnableIRQ(USART3_IRQn);
}

int main(void) {


	UART();
	UART_IRQ();
	init();

    SystemInit();

    //Initialize LCD 20 cols x 4 rows
    TM_HD44780_Init(20, 4);

    browse_menu();


}

