#pragma once
#include<cstdio>
#include<windows.h>
#define FTD2XX_EXPORTS
#include "ftd2xx.h"

#include <chrono>
#include <thread>

#include <iostream>
using namespace std;

#define MY_LCD_DESCRIPTION "EDA LCD 2 B"
//#define MY_LCD_DESCRIPTION "EDA LCD 2 A"

#define CONNECTING_TIME 5 //in seconds


//Defincion de los pins del display.

#define PIN_P0 0
#define PIN_P1 1
#define PIN_P2 2
#define PIN_P3 3
#define PIN_P4 4
#define PIN_P5 5
#define PIN_P6 6
#define PIN_P7 7

//Representacion de cada bit.

#define LCD_E (UCHAR)(1<< (PIN_P0)) //Bit de Enable
#define LCD_RS (UCHAR)(1 << (PIN_P1)) //Bit de Register Select

#define LCD_D4 (UCHAR)(1<< (PIN_P4))  //Bits D4-D7
#define LCD_D5 (UCHAR)(1 << (PIN_P5))
#define LCD_D6 (UCHAR)(1<< (PIN_P6))
#define LCD_D7 (UCHAR)(1 << (PIN_P7))

//Instrucciones del LCD
#define LCD_CLEAR ((UCHAR)0x01)
#define LCD_RETURN_HOME ((UCHAR)0x02)
#define LCD_ENTRY_MODE_SET ((UCHAR)0x04)
#define LCD_CURSOR_DISPLAY_SHIFT ((UCHAR)0x10)
#define LCD_FUNCTION_SET ((UCHAR)0x20)
#define LCD_SET_CGRAM_ADRESS ((UCHAR)0x40)
#define LCD_SET_DDRAM_ADRESS ((UCHAR)0x80)
#define LCD_DISPLAY_CONTROL ((UCHAR)0x08)

//Mascaras.
#define SET_IR_ON 0x00
#define SET_DR_ON 0x02
#define SET_ENABLE_ON (UCHAR)(LCD_E)
#define SET_ENABLE_OFF (UCHAR)(~(LCD_E))

#define SET_LCD_D4_ON (UCHAR)(LCD_D4)			//Activos altos
#define SET_LCD_D5_ON (UCHAR)(LCD_D5)
#define SET_LCD_D6_ON (UCHAR)(LCD_D6)
#define SET_LCD_D7_ON (UCHAR)(LCD_D7)

#define SET_LCD_D4_OFF (UCHAR)(LCD_D4 ^ LCD_D4)	//Activos altos  
#define SET_LCD_D5_OFF (UCHAR)(LCD_D5 ^ LCD_D5)
#define SET_LCD_D6_OFF (UCHAR)(LCD_D6 ^ LCD_D6)
#define SET_LCD_D7_OFF (UCHAR)(LCD_D7 ^ LCD_D7)

#define MASK_LEDS (UCHAR)(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)
#define NOT_MASK_LEDS (UCHAR)(~(MASK_LEDS))

#define LCD_MASKED_D4 (UCHAR)(~(LCD_D4))
#define LCD_MASKED_D5 (UCHAR)(~(LCD_D5))
#define LCD_MASKED_D6 (UCHAR)(~(LCD_D6))
#define LCD_MASKED_D7 (UCHAR)(~(LCD_D7))

using namespace std::this_thread;
using namespace std::chrono_literals;
using std::chrono::system_clock;

//Inicializa el FTDI y el LCD
FT_HANDLE * lcdInit(int iDevice);

//Escribe un byte al IR
void lcdWriteIR(FT_HANDLE * deviceHandler, unsigned char valor);

//Escribe un byte al DR
void lcdWriteDR(FT_HANDLE * deviceHandler, unsigned char valor);


