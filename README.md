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
   * menu.c
     + Pump library use
       ```c
       void kociolek(void){
        pump(rum, 4);
        pump(orange, 5);
        pump(lemon, 5);
        pump(vodka, 3);
       }
       ``` 
         >Simple, isn't it? You specify liquid and amount in centiliters. Thus minimal amount is 10 mililiters.
     + Pump library explained
         >Our machine has 4 pumps. Each pump has it's own timer.
         ```c
         TIM_TypeDef* LEMON_TIMER = TIM7;
         TIM_TypeDef* ORANGE_TIMER = TIM3;
         TIM_TypeDef* RUM_TIMER = TIM4;
         TIM_TypeDef* VODKA_TIMER = TIM5;
         ```
         >Thanks to separete timers evry pump is independent from another pumps. Each pump is handled by separete interruption.
         ```c
         const uint8_t LEMON_IRQ = TIM7_IRQn;
         const uint8_t ORANGE_IRQ = TIM3_IRQn;
         const uint8_t RUM_IRQ = TIM4_IRQn;
         const uint8_t VODKA_IRQ = TIM5_IRQn;

         void lemon_IRQHandler();
         void orange_IRQHandler();
         void rum_IRQHandler();
         void vodka_IRQHandler();

         void TIM7_IRQHandler(){lemon_IRQHandler();}
         void TIM3_IRQHandler(){orange_IRQHandler();}
         void TIM4_IRQHandler(){rum_IRQHandler();}
         void TIM5_IRQHandler(){vodka_IRQHandler();}
         ```
         >One timer cycle means that 10 mililiters were pumped.
         ```c
         const int PERIOD = 9999;
         const int PRESCALER = 2748;
         ```
         >Every pump starts pumping when invoked. Starting pump means to start it's timer and sends low signal to pin connected to relay. 
         ```c
         void start_lemon_pump()
         {
	   is_lemon_pumping = true;
	   TIM_Cmd(LEMON_TIMER,ENABLE);
	   GPIO_ResetBits(GPIOE,LEMON_PIN);
         }
         ```
         >Pump stop is handled in interrupt handler. When counter is equal to desired amount pump stops.
         ```c
         void lemon_IRQHandler()
         {
           if (TIM_GetITStatus(LEMON_TIMER, TIM_IT_Update) != RESET)
	   {
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
	 ```
- Bluetooth handling
  * main.c
    + USART3_IRQHandler - receiving a bluetooth signal
    
      ```c
      void USART3_IRQHandler(void)
      {
          if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
          {
                  if (USART3->DR == 'B' )
                  {
                      bacardi();
                  }
                  if (USART3->DR == 'CL' )
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
                  if (USART3->DR == 'I' )
                  {
                     info();
                  }
          }
      }
      ```  
    + UART initialization
    
      ```c
      void initUart(){
          RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE); //uart
          RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE); //rxd txd

          // konfiguracja linii Rx i Tx
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

      void initUartIrq(){
          //nvic configuration structure
          NVIC_InitTypeDef NVIC_InitStructure2;
          // turning on the interrupt for reciving data
          USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
          NVIC_InitStructure2.NVIC_IRQChannel = USART3_IRQn;
          NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0;
          NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0;
          NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
          NVIC_Init(&NVIC_InitStructure2);
          // turning on UART intterupt
          NVIC_EnableIRQ(USART3_IRQn);
      }
      ```  

- External libraries
  * tm_stm32f4_delay
  * tm_stm32f4_gpio
  * tm_stm32f4_timer_properties
  * tm_stm32f4_hd44780

# Usage of drink machine
  * Plug both power cables into power cord
  * Turn on ATX power supply
  * Menu will start automaticaly
    > You can navigate menu with buttons located outside casing
  * Inside our machine is also hc-05 bluetooth module
    > To connect to drink machine we recommend to use [Bluetooth Terminal](https://play.google.com/store/apps/details?id=com.dev.aproschenko.bluetoothterminal) app 
  * Description of transmitting signals ( send single character )
    > B -> bacardi();
    >> CL -> cuba_libre();
    >>> V -> vodka_shot();
    >>>> N -> kociolek();
    >>>>> I -> info();

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
