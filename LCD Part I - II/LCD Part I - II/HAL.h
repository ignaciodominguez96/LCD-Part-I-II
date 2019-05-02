#pragma once
#include<cstdio>
#include<windows.h>
#define FTD2XX_EXPORTS

#include <chrono>
#include <thread>
#include <iostream>

#include "ftd2xx.h"


using namespace std;


#error cambiar descripcion
#define MY_LCD_DESCRIPTION "EDA LCD X X"

#define UCHAR 

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


//Mascaras.
#define SET_IR_ON 0x00
#define SET_DR_ON 0x02
#define SET_ENABLE_ON ((UCHAR)(LCD_E))
#define SET_ENABLE_OFF ((UCHAR)(~(LCD_E)))

#define SET_LCD_D4_ON ((UCHAR)(LCD_D4))			//Activos altos
#define SET_LCD_D5_ON ((UCHAR)(LCD_D5))
#define SET_LCD_D6_ON ((UCHAR)(LCD_D6))
#define SET_LCD_D7_ON ((UCHAR)(LCD_D7))

#define SET_LCD_D4_OFF ((UCHAR)(LCD_D4 ^ LCD_D4))	//Desactivar bajos  
#define SET_LCD_D5_OFF ((UCHAR)(LCD_D5 ^ LCD_D5))
#define SET_LCD_D6_OFF ((UCHAR)(LCD_D6 ^ LCD_D6))
#define SET_LCD_D7_OFF ((UCHAR)(LCD_D7 ^ LCD_D7))

#define MASK_LEDS ((UCHAR)(LCD_D4 | LCD_D5 | LCD_D6 | LCD_D7)) //elegir solamente data
#define NOT_MASK_LEDS ((UCHAR)(~(MASK_LEDS)))		 //elegir todo menos data

#define LCD_MASKED_NOT_D4 ((UCHAR)(~(LCD_D4)))		//elegir todo menos data4
#define LCD_MASKED_NOT_D5 ((UCHAR)(~(LCD_D5)))		//elegir todo menos data5
#define LCD_MASKED_NOT_D6 ((UCHAR)(~(LCD_D6)))		//elegir todo menos data6
#define LCD_MASKED_NOT_D7 ((UCHAR)(~(LCD_D7)))		//elegir todo menos data7


#define BIT0	((UCHAR)1<<0)	//en instrucions DB0
#define BIT1	((UCHAR)1<<1)	//en instrucions DB1
#define BIT2	((UCHAR)1<<2)	//en instrucions DB2
#define BIT3	((UCHAR)1<<3)	//en instrucions DB3
#define BIT4	((UCHAR)1<<4)	//en instrucions DB4
#define BIT5	((UCHAR)1<<5)	//en instrucions DB5
#define BIT6	((UCHAR)1<<6)	//en instrucions DB6
#define BIT7	((UCHAR)1<<7)	//en instrucions DB7
#define BIT8	((UCHAR)1<<8)	//en instrucions R/W'
#define BIT9	((UCHAR)1<<9)	//en instrucions RS




//Instrucciones del LCD
#define LCD_CLEAR							((UCHAR)(BIT0))
#define LCD_RETURN_HOME						((UCHAR)(BIT1))
#define LCD_ENTRY_MODE_SET					((UCHAR)(BIT2))
#define LCD_DISPLAY_CONTROL					((UCHAR)(BIT3))
#define LCD_CURSOR_DISPLAY_SHIFT			((UCHAR)(BIT4))
#define LCD_FUNCTION_SET					((UCHAR)(BIT5))
#define LCD_SET_CGRAM_ADRESS				((UCHAR)(BIT6))
#define LCD_SET_DDRAM_ADRESS				((UCHAR)(BIT7))

//las siguientes instrucciones no se va a usar pero las escribo igual
#define LCD_READ_BUSY_FLAG_AND_ADDRESS		((UCHAR)(BIT8))
#define	LCD_WRITE_DATA_TO_CG_OR_DDRAM		((UCHAR)(BIT9))									
#define	LCD_READ_DATA_FROM_CG_OR_DDRAM		((UCHAR)(BIT9|BIT8))

using namespace std::this_thread;
using namespace std::chrono_literals;
using std::chrono::system_clock;

//Inicializa el FTDI y el LCD
FT_HANDLE * lcdInit(int iDevice);

//Escribe un byte al IR
void lcdWriteIR(FT_HANDLE * deviceHandler, unsigned char valor);

//Escribe un byte al DR
void lcdWriteDR(FT_HANDLE * deviceHandler, unsigned char valor);


