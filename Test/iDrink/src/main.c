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
const char menu_002[] = "  Ustawienia      "; //2
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
		{menu_002, 5, 1, 3, 2, 0},
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
	pump(rum, 80);
	pump(orange, 80);
}

void cuba_libre(void){
	pump(rum, 60);
	pump(orange, 60);
	pump(lemon, 60);
}

void vodka_shot(void){
	pump(vodka, 40);
}
void vodka_juice(void){
	pump(vodka, 80);
	pump(lemon, 80);
}
void vodka_shot_juice(void){
	pump(vodka, 20);
	pump(lemon, 20);
}
void vodka_cola(void){
	pump(vodka, 80);
	pump(orange, 80);
}
void kociolek(void){
	pump(rum, 40);
	pump(orange, 50);
	pump(lemon, 50);
	pump(vodka, 30);
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
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	init_pump_driver();

	GPIO_InitTypeDef Przyciski;
	Przyciski.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	Przyciski.GPIO_Mode = GPIO_Mode_IN;
	Przyciski.GPIO_OType = GPIO_OType_PP;
	Przyciski.GPIO_Speed = GPIO_Speed_100MHz;
	Przyciski.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &Przyciski);

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin =
	GPIO_Pin_12 | GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

int main(void) {

	init();
    //Initialize system
    SystemInit();

    //Initialize LCD 20 cols x 4 rows
    TM_HD44780_Init(20, 4);

    browse_menu();

}

