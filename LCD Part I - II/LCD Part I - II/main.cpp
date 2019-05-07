#include <iostream>
#include <string>
#include <chrono>
#include "HAL.h"
#include "HitachiLCD.h"


#define DELAY_2SEG	2000ms

#define ERROR_INIT -1



int main(void)
{
	HitachiLCD lcd;

	
	if (lcd.lcdInitOk() == false)
	{
		cout << "Error en la incializacion." << endl;
		return ERROR_INIT;
	}


	char letter = 0;
	cursorPosition pos;

	cout << "Bienvenido al TP7 - Grupo 4 - LCD Fase I y II" << endl << endl;

	cout << "Integrantes del grupo:" << endl;
	cout << "-DIMAS MIGUEL BOSCH" << endl;
	cout << "-IGNACIO DOMINGUEZ" << endl;
	cout << "-SEBASTIAN NAHUEL ROSSI" << endl;

	cout << endl;

	cout << "Instrucciones:" << endl;
	cout << "Escriba cualquier palabra o texto y luego presione ENTER, para mostrar el mismo en el LCD" << endl;
	cout << "Para usar un comando debe anteponer el '-' y luego el comando (letra) correspondiente" << endl << endl;



	cout << "Comandos:" << endl;
	cout << "Presione 'w' para mover el cursor arriba" << endl;
	cout << "Presione 's' para mover el cursor abajo" << endl;
	cout << "Presione 'a' para mover el cursor a la izquierda" << endl;
	cout << "Presione 'd' para mover el cursor a la derecha" << endl;
	cout << "Presione 'c' para borrar" << endl;
	cout << "Presione 'x' para borrar todo" << endl;
	cout << "Presione 'g' para ver la posicion del cursor en la linea de comandos" << endl;
	cout << "Presione 'q' para salir" << endl << endl;
	
	cout << "Ejemplo: Escribir ' -g ' para mostrar por pantalla de comandos la posicion del cursor " << endl << endl;

	lcd << " ";
	lcd << " ";
	lcd.lcdClear();

	bool exit_while = false;
	

	do	{
		
		letter = getchar();
		
		
		if ( (letter != '\n') && (letter != '-') )
		{

			lcd << letter;

		}

		if (letter == '-')
		{
			letter = getchar();

			switch (letter)
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
					cout << "Columna: " << pos.col <<", " << "Fila: " << pos.row << endl;
					break;
				case 'q':
					exit_while = true;
					break;
		}
		}

		




	} while ( exit_while == false);





	cout << "Prueba de mensajes con el operador '<<' con los tres tipos de datos" << endl;
	lcd.lcdClear();


	const char ptr_char[] = "Con * const Char";         //guarda el mensaje que luego va a mostrar el display en un arreglo

	lcd << ptr_char;
	this_thread::sleep_for(DELAY_2SEG);			//se prueban los 3 formas para pasarle a lcd <<
	lcd.lcdClear();

	lcd << "Con const Char"; 					//se le pasa directamente el mensaje a lcd <<
	this_thread::sleep_for(DELAY_2SEG);
	lcd.lcdClear();

	string str = "Con String";					//se guarda un tipo string para luego mostralo en el display
	lcd << str;
	this_thread::sleep_for(DELAY_2SEG);
	
	lcd.lcdClear();
	this_thread::sleep_for(DELAY_2SEG);
	

	   	 	
	return 0;
}

