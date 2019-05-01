#include <iostream>
#include <string>
#include <chrono>
#include "HAL.h"
#include "HitachiLCD.h"

int main(void)
{
	HitachiLCD lcd(2);
	if (lcd.lcdInitOk() == false)
	{
		cout << "Error en la incializacion." << endl;
		return -1;
	}
	char c = 0;
	cursorPosition pos;
	std::cout << "Press W to Move Cursor Up" << std::endl;
	std::cout << "Press A to Move Cursor Left" << std::endl;
	std::cout << "Press S to Move Cursor Down" << std::endl;
	std::cout << "Press D to Move Cursor Right" << std::endl;
	std::cout << "Press C to Clear" << std::endl;
	std::cout << "Press X to Clear to End of Line" << std::endl;
	std::cout << "Press G  to print Cursor Position in cmd" << std::endl;
	std::cout << "Press Q to exit" << std::endl;
	std::cout << "Press any other letter to print to LCD" << std::endl;

	do
	{
		c = getchar();
		if (c != '\n')
		{
			switch (c)
			{
			case 'd':
				lcd.lcdMoveCursorRight();
				break;
			case 'w':
				lcd.lcdMoveCursorUp();
				break;
			case 'a':
				lcd.lcdMoveCursorLeft();
				break;
			case 's':
				lcd.lcdMoveCursorDown();
				break;
			case 'c':
				lcd.lcdClear();
				break;
			case 'x':
				lcd.lcdClearToEOL();
				break;
			case 'g':
				pos = lcd.lcdGetCursorPosition();
				std::cout << "Column: " << pos.column << std::endl;
				std::cout << "Row: " << pos.row << endl;
				break;
			case 'q':
				break;
			default:
				lcd << c;
				break;
			}
		}
	} while (c != 'q');
	std::cout << "Begining of strings test in LCD..." << std::endl;
	sleep_for(10ms);
	const char h[] = "demo de const char * ";
	lcd << h;
	lcd.lcdClear();
	lcd << "demo de cstring.......";
	sleep_for(10ms);
	string str = "demo de str_c++";
	lcd.lcdClear();
	lcd << str;
	sleep_for(10ms);
	lcd.lcdClear();
	sleep_for(50ms);
	std::cout << "Press enter to Quit..." << std::endl;
	return 0;
}