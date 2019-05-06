#include <iostream>
#include <string>
#include <chrono>
#include "HAL.h"
#include "HitachiLCD.h"


void get_despcription_connected(void);

int main(void)
{
	HitachiLCD lcd;

	//get_despcription_connected(void);

using namespace std;



	if (lcd.lcdInitOk() == false)
	{
		cout << "Error en la incializacion." << endl;
		getchar();
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


	lcd << " ";
	lcd << " ";
	lcd.lcdClear();


	do	{
		
		c = getchar();
		pos = lcd.lcdGetCursorPosition();

		if ((pos.row == 2) && (pos.column == 15))
		{
			std::cout << "entre" << endl;
		}
		else if ((pos.row == 2) && (pos.column == 0))
		{
			std::cout << "entre" << endl; 
		}
		
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
				
				pos = lcd.lcdGetCursorPosition();
				cout << "Letter:" << c << " , Row: " << pos.row << " , Column: " << pos.column << endl;
				cout  << endl;

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
	this_thread::sleep_for(1000ms);
	string str = "demo de str_c++";
	lcd.lcdClear();
	lcd << str;
	this_thread::sleep_for(1000ms);
	lcd.lcdClear();
	this_thread::sleep_for(5000ms);
	std::cout << "Press enter to Quit..." << std::endl;
	return 0;
}




void get_despcription_connected(void)
{
	FT_STATUS ftStatus;
	DWORD numDevs;



	ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
	if (ftStatus == FT_OK) {

		cout << "Get the number of devices currently connected  " << endl;

		cout << numDevs << endl;


	}
	else {
		// FT_ListDevices failed 
	}


	DWORD devIndex = 0; // first device 
	char Buffer[64]; // more than enough room! 

	ftStatus = FT_ListDevices((PVOID)devIndex, Buffer, FT_LIST_BY_INDEX | FT_OPEN_BY_DESCRIPTION);
	if (ftStatus == FT_OK) {

		cout << "Get serial number of first device " << endl;

		cout << Buffer << endl;

	}
	else {
		// FT_ListDevices failed 
	}




	char *BufPtrs[3];   // pointer to array of 3 pointers 
	char Buffer1[64];      // buffer for description of first device  
	char Buffer2[64];      // buffer for description of second device 

	// initialize the array of pointers 
	BufPtrs[0] = Buffer1;
	BufPtrs[1] = Buffer2;
	BufPtrs[2] = NULL;      // last entry should be NULL 

	ftStatus = FT_ListDevices(BufPtrs, &numDevs, FT_LIST_ALL | FT_OPEN_BY_DESCRIPTION);
	if (ftStatus == FT_OK) {

		cout << "Get device descriptions of all devices currently connected " << endl;

		cout << Buffer1 << endl;
		cout << Buffer2 << endl;

	}
	else {
		// FT_ListDevices failed 
	}



}

