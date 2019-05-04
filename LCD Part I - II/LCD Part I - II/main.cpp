#include <iostream>
#include <string>
#include <chrono>
#include "HAL.h"
#include "HitachiLCD.h"

using namespace std;


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
	cout << "Press W to Move Cursor Up" << endl;
	cout << "Press A to Move Cursor Left" << endl;
	cout << "Press S to Move Cursor Down" << endl;
	cout << "Press D to Move Cursor Right" << endl;
	cout << "Press C to Clear" << endl;
	cout << "Press X to Clear to End of Line" << endl;
	cout << "Press G  to print Cursor Position in cmd" << endl;
	cout << "Press Q to exit" << endl;
	cout << "Press any other letter to print to LCD" << endl;

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
				cout << "Column: " << pos.column << endl;
				cout << "Row: " << pos.row << endl;
				break;
			case 'q':
				break;
			default:
				lcd << c;
				break;
			}
		}
	} while (c != 'q');
	cout << "Begining of strings test in LCD..." << endl;
	this_thread::sleep_for(10ms);
	const char h[] = "demo de const char * ";
	lcd << h;
	lcd.lcdClear();
	lcd << "demo de cstring.......";
	this_thread::sleep_for(10ms);
	string str = "demo de str_c++";
	lcd.lcdClear();
	lcd << str;
	this_thread::sleep_for(10ms);
	lcd.lcdClear();
	this_thread::sleep_for(50ms);
	std::cout << "Press enter to Quit..." << std::endl;
	return 0;
}