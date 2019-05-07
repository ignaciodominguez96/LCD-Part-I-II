#include <iostream>
#include <string>
#include <chrono>
#include "HAL.h"
#include "HitachiLCD.h"

using namespace std;


void print_lcd_pause(HitachiLCD * lcd, string str);


int main(void)
{
	HitachiLCD lcd;

	



	if (lcd.lcdInitOk() == false)
	{
		cout << "Error en la incializacion." << endl;
		getchar();
		return -1;
	}


	char c = 0;
	cursorPosition pos;
	cout << "Presione W para mover el cursor arriba" << endl;
	cout << "Presione S para mover el cursor abajo" << endl;
	cout << "Presione A para mover el cursor a la izquierda" << endl;
	cout << "Presione D para mover el cursor a la derecha" << endl;
	cout << "Presione C para borrar" << endl;
	cout << "Presione X para borrar todo" << endl;
	cout << "Presione G para ver la posicion del cursos en la linea de comandos" << endl;
	cout << "Presione Q para salir" << endl;
	cout << "Presione cualquier otra tecla para mostrar el mensaje en el LCD" << endl;


	lcd << " ";
	lcd << " ";
	lcd.lcdClear();
	

	do	{
		
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
				cout << "Columna: " << pos.col << endl;
				cout << "Fila: " << pos.row << endl;
				break;
			case 'q':
				break;
			default:
				
				pos = lcd.lcdGetCursorPosition();
				cout << "Fila: " << pos.row << " , Columna: " << pos.col << endl;
			
				lcd << c;

				break;
			}
		}
	} while (c != 'q');





	cout << "comienza prueba de mensajes" << endl;
	this_thread::sleep_for(10ms);						//se prueban los 3 formas para pasarle a lcd <<
	const char h[] = "puntero a const char";          //guarda el mensaje que luego va a mostrar el display en un arreglo
	lcd << h;
	lcd.lcdClear();
	lcd << "probando mensaje"; 					//se le pasa directamente el mensaje a lcd <<
	this_thread::sleep_for(1000ms);
	string str = "probando tipo string";			//se guarda un tipo string para luego mostralo en el display
	lcd.lcdClear();
	lcd << str;
	this_thread::sleep_for(1000ms);
	lcd.lcdClear();
	this_thread::sleep_for(5000ms);
	

	   	 	
	return 0;
}



void print_lcd_pause(HitachiLCD * lcd, string str)
{
	lcd->lcdClear();
	(*lcd) << str;
	this_thread::sleep_for(1000ms);

}