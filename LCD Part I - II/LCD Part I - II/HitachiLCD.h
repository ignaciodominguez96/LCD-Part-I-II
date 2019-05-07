#pragma once

#include <iostream>
#include <string>
//Modulo que contiene la estructura del  display Hitachi HD44780. 

#include "BasicLCD.h"
#include "HAL.h"

class HitachiLCD : public basicLCD
{
public:
	HitachiLCD(void);
	virtual ~HitachiLCD();
	virtual bool lcdInitOk();
	virtual FT_STATUS lcdGetError();
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
	bool can_init;
	FT_HANDLE * device_handler;
	virtual void lcdUpdateCursor();
};