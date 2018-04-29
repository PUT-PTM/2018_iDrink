/*
 * pump_driver.c
 *
 *  Created on: 29.04.2018
 *      Author: Bartek
 */

#include "pump_driver.h"

void init_pump_driver()
{

}

void pump(enum liquid liq, int amount_ml)
{
	switch (liq) {
		case l1:
			break;
		default:
			break;
	}
}

void pump_internal(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, int amount_ml)
{

}
