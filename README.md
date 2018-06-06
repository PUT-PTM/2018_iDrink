# iDrink 2018

iDrink is an student's project. 
What we are trying to do here is to build a drink machine! :tropical_drink:

> To me "drink responsibilty" means don't spill it.
~ Wise old chinese man

# Code explanation
- Display
  * menu.h
    + menu structure
    
      ```c
      typedef const struct MenuStructure
        {
          const char *text;              /* name of the menu part */
          unsigned char num_menupoints;  /* how many elements this menu part has */
          unsigned char up;              /* index of upper element */
          unsigned char down;            /* index of lower element */
          unsigned char enter;           /* index of element when clicked */
          void (*fp) (void);             /* function redirect when clicked */
        } MenuEntry; 
      ```    
    + functions declarations
      ```c
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
      ``` 
   * menu.c
     + create menu
       ```c
        const char menu_000[] = "     [Menu]       "; //0
        const char menu_001[] = "  Wybierz drink   "; //1
        const char menu_002[] = "  Ustawienia      "; //2
        const char menu_003[] = "  Wykonawcy       "; //3
        const char menu_004[] = "                  "; //4
       ``` 
     + define menu structure
       ```c
        MenuEntry menu[] =
        {
            {menu_000, 5, 0, 0, 0, 0},
            {menu_001, 5, 1, 2, 6, 0},
            {menu_002, 5, 1, 3, 2, 0},
            {menu_003, 5, 2, 4, 3, info},
            {menu_004, 5, 3, 4, 4, start},                
        }; 
       ``` 
     + show menu function 
         > It is more complicated to explain how we navigate the menu ,but
         > basically we are using all the static values from created menu structure
         > and then consider all the cases.
         > 
         > We also display cursor.
         
     + browse menu function (infinite loop :arrows_counterclockwise: )
       ```c
        void browse_menu(void){
          do{
            show_menu();  //adjust display to user changes
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
            Delayms(150);  //debounc

          }while(1);
        }
       ``` 


- Pumps library

- External libraries
  * tm_stm32f4_delay
  * tm_stm32f4_gpio
  * tm_stm32f4_timer_properties
  * tm_stm32f4_hd44780

# Photos
![](https://i.imgur.com/nJXl1RL.jpg) 
![](https://i.imgur.com/CleiBFQ.jpg) 
![](https://i.imgur.com/ykVvEBY.jpg) 

# Authors
- [Szymon Janowski](https://github.com/sleter)
- [Bartosz Mikulski](https://github.com/bartmiki)

# License
MIT

![](http://eduroam.put.poznan.pl/images/put3_logo.png) 
