#include "main.h"

#ifndef TM1638_H_
#define TM1638_H_
#define LED_1 1<<0
#define LED_2 1<<1
#define LED_3 1<<2
#define LED_4 1<<3
#define LED_5 1<<4
#define LED_6 1<<5
#define LED_7 1<<6
#define LED_8 1<<7


void tm1638_Init(u8 brightness);
void led_Set(u8 led_Num, u8 state);
void leds_Set(u8 state);
void segmentN_Set(u8 seg_Num, u8 data);
void segments_Set(u8 start_Seg, u8 stop_Seg, u8 data[]);
void display_Digit(int digit,u8 pos);
void tm1638_Clear(u8 parameter);
u8 read_Keys(void);


#endif 
