/*
 * menu.h
 *
 *  Created on: 13.04.2018
 *      Author: N0tE
 */

#ifndef MENU_H_
#define MENU_H_

typedef const struct MenuStructure
		{
			const char *text;
			unsigned char num_menupoints;
			unsigned char up;
			unsigned char down;
			unsigned char enter;
			void (*fp) (void);
		} MenuEntry;

void show_menu(void);
void browse_menu(void);
void start(void);
void info(void);


void bacardi(void);
void cuba_libre(void);
void vodka_shot(void);
void vodka_juice(void);
void vodka_shot_juice(void);
void vodka_cola(void);
void kociolek(void);


#endif /* MENU_H_ */
