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

const char menu_000[] = "  [Menu]          "; //0
const char menu_001[] = "  Wybierz drink   "; //1
const char menu_002[] = "  Ustawienia      "; //2
const char menu_003[] = "  Wyjscie         "; //3
const char menu_004[] = "  func_test       "; //4

const char menu_100[] = "  [SubMenu]       "; //5
const char menu_101[] = "  Drink1          "; //6
const char menu_102[] = "  Drink2          "; //7
const char menu_103[] = "  Drink3          "; //8
const char menu_104[] = "  Drink4          "; //9
const char menu_105[] = "  Wroc            "; //10


MenuEntry menu[] =
{
		{menu_000, 5, 0, 0, 0, 0},
		{menu_001, 5, 1, 2, 6, 0},
		{menu_002, 5, 1, 3, 2, 0},
		{menu_003, 5, 2, 4, 3, 0},
		{menu_004, 5, 3, 4, 4, start},

		{menu_100, 6, 0, 0, 0, 0},
		{menu_101, 6, 6, 7, 6, 0},
		{menu_102, 6, 6, 8, 7, 0},
		{menu_103, 6, 7, 9, 8, 0},
		{menu_104, 6, 8, 10, 9, 0},
		{menu_105, 6, 9, 10, 1, 0}
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

