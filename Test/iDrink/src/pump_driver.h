/*
 * pump_driver.h
 *
 *  Created on: 29.04.2018
 *      Author: Bartek
 *
 *      B2,B7,B12,B13,A1,A2,A3,C12,C13,
 */

#ifndef PUMP_DRIVER_H_
#define PUMP_DRIVER_H_

#include "stm32f4xx.h"
#include "stm32f4_discovery.h"

enum bool {true, false};
enum liquid {lemon,orange,rum,vodka};

void init_pump_driver();
void pump(enum liquid liq, int amount_ml);

void start_lemon_pump();
void start_orange_pump();
void start_rum_pump();
void start_vodka_pump();


#endif /* PUMP_DRIVER_H_ */
