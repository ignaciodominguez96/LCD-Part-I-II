#pragma once

#define FTD2XX_EXPORTS
#include "ftd2xx.h"
#include <iostream>
#include <string>
//Modulo que contiene la estructura del  display Hitachi HD44780. 

#include "BasicLCD.h"
#include "HAL.h"

class HitachiLCD : public basicLCD
{
public:
	HitachiLCD(int iDevice);
	virtual ~HitachiLCD();
	virtual bool lcdInitOk();
	virtual bool lcdGetError();
	virtual bool lcdClear();
	virtual bool lcdClearToEOL();
	virtual basicLCD& operator<<(const char c);
	virtual basicLCD& operator<<(const char * c);
	virtual basicLCD& operator<<(std::string str);
	virtual bool lcdMoveCursorUp();
	virtual bool lcdMoveCursorDown();
	virtual bool lcdMoveCursorRight();
	virtual bool lcdMoveCursorLeft();
	virtual bool lcdSetCursorPosition(const cursorPosition pos);
	virtual cursorPosition lcdGetCursorPosition();

private:
	bool Init;
	FT_HANDLE * device_handler;
	virtual void lcdUpdateCursor();
	unsigned char Hcadd();
};