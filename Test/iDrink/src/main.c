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

int main(void) {
    //Rectangle for custom character
    //xxx means doesn't care, lower 5 bits are important for LCD
    uint8_t customChar[] = {
        0x1F,    // xxx 11111
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x11,    // xxx 10001
        0x1F    // xxx 11111
    };
    //Initialize system
    SystemInit();

    //Initialize LCD 20 cols x 4 rows
    TM_HD44780_Init(16, 2);

    //Save custom character on location 0 in LCD
    TM_HD44780_CreateChar(0, &customChar[0]);

    //Put string to LCD
    TM_HD44780_Puts(0, 0, "    cos");


    //Show custom character at x = 1, y = 2 from RAM location 0
    TM_HD44780_PutCustom(1, 2, 1);

    while (1) {

    }
}

