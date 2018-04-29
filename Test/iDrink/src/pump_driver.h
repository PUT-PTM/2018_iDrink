/*
 * pump_driver.h
 *
 *  Created on: 29.04.2018
 *      Author: Bartek
 */

#ifndef PUMP_DRIVER_H_
#define PUMP_DRIVER_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

enum liquid {l1,l2,l3,l4};

void init_pump_driver();

void pump(enum liquid liq, int amount_ml);

void pump_internal(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, int amount_ml);


#endif /* PUMP_DRIVER_H_ */
