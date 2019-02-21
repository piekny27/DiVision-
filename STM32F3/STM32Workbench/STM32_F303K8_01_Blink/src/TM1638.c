#include "TM1638.h"
//#include "stm32f103xb.h"
#include "stm32f3xx.h"
#define ADDR_AUTO  0x40
#define ADDR_FIXED 0x44
#define REG_READ 0x42 	
#define STARTSEGADDR  0xc0//
#define STARTLEDADDR  0xc1 //
#define INDEX_NEGATIVE_SIGN	16
#define INDEX_BLANK			    17 // 17 nothing ,0  zero beter for clock

//#define CS_H (GPIOB->BSRR = ((uint16_t)0x1000))
//#define CS_L (GPIOB->BRR = ((uint16_t)0x1000)) 
#define CS_H (GPIOA->BSRR = ((uint16_t)0x0010))
#define CS_L (GPIOA->BRR =((uint16_t)0x0010)) 

u8 BlankingFlag;
u8 SegArray[8];
u8  reg_mas[4];
//      A
//     ---
//  F |   | B
//     -G-
//  E |   | C
//     ---
//      D
const u8 digitToSegment[] = {
  0x3f,    // 0 0b00111111
  0x06,    // 1 0b00000110
  0x5b,    // 2 0b01011011
  0x4f,    // 3 0b01001111
  0x66,    // 4 0b01100110
  0x6d,    // 5 0b01101101
  0x7d,    // 6 0b01111101
  0x07,    // 7 0b00000111
  0x7f,    // 8 0b01111111
  0x6f,    // 9 0b01101111
  0x77,    // A 0b01110111
  0x7c,    // b 0b01111100
  0x39,    // C 0b00111001
  0x5e,    // d 0b01011110
  0x79,    // E 0b01111001
  0x71,    // F 0b01110001
  0x40,    // - 0b01000000
  0x00,     // nothing 0b00000000
  0x80     // dot 
  };

//=========================================================================//
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
//=========================================================================//
void tm1638_Init(u8 brightness){
	if(brightness>7)return;
	CS_L;
	spi_Send(0x88|brightness);//0x88 Đ˛ĐşĐ»ŃŽŃ‡Đ°ĐµĐĽ Ń€ĐµĐ¶Đ¸ĐĽ ĐşĐľĐĽĐ°Đ˝Đ´ Đ¸ Ń�Đ°ĐĽ Đ´Đ¸Ń�ĐżĐ»ĐµĐą
	CS_H;
	tm1638_Clear(0);
}
//=========================================================================//
//++++++++++++++++++++++Ń�ĐżŃ€Đ°Đ˛Đ»ĐµĐ˝Đ¸Đµ ĐľĐ´Đ˝Đ¸ĐĽ Ń�Đ˛ĐµŃ‚ĐľĐ´Đ¸ĐľĐ´ĐľĐĽ+++++++++++++++++++++++//
//=========================================================================//
void led_Set(u8 led_Num, u8 state){//led_Num 1-8
	if(led_Num>8 || led_Num<1)return;
	CS_L;
	spi_Send(ADDR_FIXED);  
	CS_H;

	CS_L;
	spi_Send(STARTLEDADDR+(led_Num-1)*2);
	spi_Send(state);
	CS_H;
}
//=========================================================================//
//+++++++++++++++++++++++Ń�ĐżŃ€Đ°Đ˛Đ»ĐµĐ˝Đ¸Đµ Đ˛Ń�ĐµĐĽĐ¸ Ń�Đ˛ĐµŃ‚ĐľĐ´Đ¸ĐľĐ´Đ°ĐĽĐ¸+++++++++++++++++++++//
//=========================================================================//
void leds_Set(u8 state){//+
	u8 led_Num = 0;
	CS_L;
	spi_Send(ADDR_FIXED);  
	CS_H;
 for(;led_Num<8;led_Num++){
	CS_L;
	spi_Send(STARTLEDADDR+led_Num*2);
	spi_Send(state&0x01);
	state >>= 1; 
	CS_H;
 }
}
//=========================================================================//
//++++++++++++++++++++++++ĐľŃ‚ĐľĐ±Ń€Đ°Đ¶Đ°ĐµŃ‚ i-Ń‹Đą Ń�ĐµĐłĐĽĐµĐ˝Ń‚++++++++++++++++++++++++++//
//=========================================================================//
void segmentN_Set(u8 seg_Num, u8 data){//seg_Num 1-8;
	if(seg_Num>8 || seg_Num<1)return;
	seg_Num=seg_Num-1;
	data=digitToSegment[data];
	CS_L;
	spi_Send(ADDR_FIXED); 
	CS_H;

	CS_L;
	spi_Send(STARTSEGADDR+seg_Num*2);//0,2,4,6,8,10....
	spi_Send(data);
	CS_H;
}

//=========================================================================//
//+++++++++++++++++++++++++ĐľŃ‚ĐľĐ±Ń€Đ°Đ¶Đ°ĐµŃ‚ ĐĽĐ°Ń�Ń�Đ¸Đ˛+++++++++++++++++++++++++++++++//
//=========================================================================//
void segments_Set(u8 start_Seg, u8 stop_Seg, u8 data[]){//+
	u8 i=0;
	CS_L;
	spi_Send(ADDR_FIXED); 
	CS_H;

		for(;start_Seg < stop_Seg;start_Seg++,i++){
			CS_L;
			spi_Send(STARTSEGADDR+start_Seg*2);
			spi_Send(data[i]);
			CS_H;
		}
}
//=========================================================================//
//+++++++++++++++++++++Ń‡Đ¸Ń‚Đ°ĐµĐĽ Ń�ĐľŃ�Ń‚ĐľŃŹĐ˝Đ¸Đµ ĐşĐ˝ĐľĐżĐľĐş+++++++++++++++++++++++++++++//
//=========================================================================//
u8 read_Keys(){//+
	u8 keys = 0; //reg_mas[4];
  u8 i=0;
	
	CS_L;
	spi_Read_Reg(reg_mas);//Ń‡Đ¸Ń‚Đ°ĐµĐĽ Ń�ĐľŃ�Ń‚ĐľŃŹĐ˝Đ¸Đµ 4 Ń€ĐµĐłĐ¸Ń�Ń‚Ń€ĐľĐ˛ 
	CS_H;
  for(;i<4;i++) keys |= (reg_mas[i]&0x11)<<i;
	return keys;
}
//=========================================================================//
//+++++++++++++++++++Ń€Đ°Đ·Đ±Đ¸Đ˛Đ°ĐµĐĽ Ń‡Đ¸Ń�Đ»Đľ Đ˝Đ° "Ń�ĐµĐłĐĽĐµĐ˝Ń‚Ń‹"+++++++++++++++++++++++++//
//=========================================================================//
void display_Digit(int digit,u8 pos)//Ń€Đ°Đ·Đ±Đ¸Đ˛Đ°ĐµĐĽ Ń†ĐµĐ»ĐľĐµ Ń‡Đ¸Ń�Đ»Đľ Đ˝Đ° ĐĽĐ°Ń�Ń�Đ¸Đ˛ Ń‡Đ¸Ń�ĐµĐ»
{
	u8 i=0;
	u8 j=0;
	u8 digit_starts=0;
	if((digit > 99999999)||(digit < -9999999))return;
	if(digit < 0)
	{
		SegArray[0] = INDEX_NEGATIVE_SIGN;
		digit = (digit & 0x7fffffff);
		SegArray[1] = digit/1000000;
		digit %= 1000000;
		SegArray[2] = digit/1000000;
		digit %= 1000000;
		SegArray[3] = digit/1000000;
		digit %= 1000000;
		SegArray[4] = digit/1000000;
		digit %= 1000000;
		SegArray[5] = digit/1000000;
		digit %= 1000000;
		SegArray[6] = digit / 10;
		SegArray[6] = digit % 10;
		if(BlankingFlag)
		{
			if(SegArray[1] == 0)
			{
				SegArray[1] = INDEX_BLANK;
				if(SegArray[2] == 0){ 
					SegArray[2] = INDEX_BLANK;
				  if(SegArray[3] == 0){ 
					   SegArray[3] = INDEX_BLANK;
						 if(SegArray[4] == 0){ 
					   SegArray[4] = INDEX_BLANK;
							 if(SegArray[5] == 0){ 
					        SegArray[5] = INDEX_BLANK;
								  if(SegArray[6] == 0){ 
					           SegArray[7] = INDEX_BLANK;
									  }
							 }
						 }
					}
				}
			}
		}
	}
	else
	{
		SegArray[0] = digit/10000000;
		digit %= 10000000;
		SegArray[1] = digit/1000000;
		digit %= 1000000;
		SegArray[2] = digit / 100000;
		digit %= 100000;
		SegArray[3] = digit / 10000;
		digit %= 10000;
		SegArray[4] = digit / 1000;
		digit %= 1000;
		SegArray[5] = digit / 100;
		digit %= 100;
		SegArray[6] = digit / 10;
		SegArray[7] = digit % 10;
		if(BlankingFlag)
		{
			if(SegArray[0] == 0)
			{
				SegArray[0] = INDEX_BLANK;
				if(SegArray[1] == 0)
				{
					SegArray[1] = INDEX_BLANK;
					if(SegArray[2] == 0){
					   SegArray[2] = INDEX_BLANK;
						 	if(SegArray[3] == 0){
								 SegArray[3] = INDEX_BLANK;
								 if(SegArray[4] == 0){
								   SegArray[4] = INDEX_BLANK;
									 if(SegArray[5] == 0){
								      SegArray[5] = INDEX_BLANK;
										 if(SegArray[6] == 0){
								        SegArray[6] = INDEX_BLANK;
										 }
									 }
								 }
							}
					}
				}
			}
		}
	}
	BlankingFlag = 1;

	while(SegArray[i]==INDEX_BLANK)i++;	
	digit_starts=(i);
	for(;i<8;i++){
	SegArray[i-digit_starts]=digitToSegment[SegArray[i]];
	SegArray[i]=0;
	}
	segments_Set(0+pos-1,(8-digit_starts)+pos-1,SegArray);//0-8
}
//=========================================================================//
//++++++++++++++++++++++++Ń‡Đ¸Ń�Ń‚Đ¸Ń‚ Đ´Đ¸Ń�ĐżĐ»ĐµĐą+++++++++++++++++++++++++++++++++++//
//=========================================================================//

void tm1638_Clear(u8 parameter){//0-all;1-segments;2-led
	u8 i=0;

	CS_L;
	if(!parameter)spi_Send(ADDR_AUTO); 
	   else spi_Send(ADDR_FIXED); 
	CS_H;

		switch(parameter){
			case 0://all
				CS_L;
				spi_Send(STARTSEGADDR); 
	     	for(; i < 16; i ++) spi_Send(0);
			  CS_H;
			break;
		  case 1://segments
				for(; i < 8; i ++){
		    CS_L;
		    spi_Send(STARTSEGADDR+i*2);
		    spi_Send(0);
		    CS_H;
	     }
			break;
			case 2://led
				for(;i<8;i++){
	         CS_L;
	         spi_Send(STARTLEDADDR+i*2);
	         spi_Send(0);
	         CS_H;
     }
			break;	
		}

}
