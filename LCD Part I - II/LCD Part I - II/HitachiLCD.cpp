#include "HitachiLCD.h"

#define ERROR_HCADD 0xFF

#define BEGIN_OF_FIRST_LINE 1			
#define END_OF_FIRST_LINE 16		
#define BEGIN_OF_SECOND_LINE 64		
#define END_OF_SECOND_LINE 80		
#define LINES_GAP (BEGIN_OF_SECOND_LINE-END_OF_FIRST_LINE)
#define TOTAL_RANGE			((cadd >= BEGIN_OF_FIRST_LINE) && (cadd <= END_OF_SECOND_LINE))
#define IS_ON_FIRST_LINE(x) (((x >= BEGIN_OF_FIRST_LINE) && (x <= END_OF_FIRST_LINE)))
#define IS_ON_SECOND_LINE(x) (((x >= BEGIN_OF_SECOND_LINE) && (x <= END_OF_SECOND_LINE)))
#define MIN_COL 1
#define MAX_COL 16
#define CANT_ROW 2
#define CANT_COL 16

HitachiLCD::HitachiLCD(int iDevice)
{
	Init = false;
	error = true;
	device_handler = lcdInit(iDevice);
	if (device_handler != nullptr)
	{
		Init = true; //Se inicializo y configuro exitosamente la inicializacion.
		error = false;
	}
	cadd = 1;
}

HitachiLCD:: ~HitachiLCD()
{
	if (Init)
	{
		FT_Close(*device_handler);
		delete device_handler;
	}

}

bool HitachiLCD::lcdInitOk()
{
	return Init;
}

FT_STATUS HitachiLCD::lcdGetError()
{
	FT_STATUS state = FT_GetStatus(*device_handler, nullptr, nullptr, nullptr);
	if (state != FT_OK)
	{
		error = true;
	}
	return state;
}

bool HitachiLCD::lcdClear()
{
	bool ret = false;
	FT_STATUS state = FT_OK;
	lcdWriteIR(device_handler, LCD_CLEAR);
	if (state == FT_OK)
	{
		cadd = 1;
		ret = true;
	}
	else
	{
		error = true;
	}
	return ret;
}

bool HitachiLCD::lcdClearToEOL()
{
	bool ret = false;
	int cadd_aux = cadd;
	int limit = 0;
	if (IS_ON_FIRST_LINE(cadd))
	{
		limit = END_OF_FIRST_LINE;
	}
	else if (IS_ON_SECOND_LINE(cadd))
	{
		limit = END_OF_SECOND_LINE;
	}
	else
	{
		std::cout << "ERROR: Cursor Overflow" << std::endl;
		error = true;
		ret = true;
	}
	if (!error)
	{
		for (; cadd <= limit ; cadd++)
		{
			lcdWriteDR(device_handler, ' '); //imprimo espacios, simulando que borro el display, letra por letra.
		}
		cadd = cadd_aux; //regreso el cursor a la posición que tenia antes de ejecutar la función.
		lcdUpdateCursor();
	}

	return ret;  //devuelve true si hubo error, false si no hubo error

}

basicLCD& HitachiLCD::operator<<(const char c)
{
	lcdWriteDR((this->device_handler), c);
	cadd++;
	if (cadd == END_OF_FIRST_LINE + 1)
	{
		cadd == BEGIN_OF_SECOND_LINE;
	}
	if (cadd == (END_OF_SECOND_LINE + 1))
	{
		cadd = BEGIN_OF_FIRST_LINE;
	}
	lcdUpdateCursor();
	return *this;
};

basicLCD& HitachiLCD::operator<<(const char * c)
{
	for (int i; c[i] != '\0'; i++)
	{
		lcdWriteDR((this->device_handler), c[i]);
		cadd++;
		if (cadd == END_OF_FIRST_LINE + 1)
		{
			cadd == BEGIN_OF_SECOND_LINE;
		}
		if (cadd == (END_OF_SECOND_LINE + 1))
		{
			cadd = BEGIN_OF_FIRST_LINE;
		}
		lcdUpdateCursor();
	}
	return *this;
};

basicLCD& HitachiLCD::operator<<(std::string str)
{
	for (unsigned int i = 0; i < str.size(); i++)
	{
		lcdWriteDR((this->device_handler), str[i]);
		cadd++;
		if (cadd == END_OF_FIRST_LINE + 1)
		{
			cadd == BEGIN_OF_SECOND_LINE;
		}
		if (cadd == (END_OF_SECOND_LINE + 1))
		{
			cadd = BEGIN_OF_FIRST_LINE;
		}
		lcdUpdateCursor();
	}

	return *this;
};

bool HitachiLCD::lcdMoveCursorUp()
{
	if (IS_ON_FIRST_LINE(cadd)) //primer línea del LCD
	{
		return false; //no se puede mover hacia arriba
	}
	else if (IS_ON_SECOND_LINE(cadd))  //segunda línea del LCD
	{
		cursorPosition newPos;
		newPos.column = 1;
		newPos.row = cadd - (LINES_GAP + CANT_COL);
		lcdSetCursorPosition(newPos);
		return true;
	}
	else   //el cadd esta fuera de rango
	{
		return false;
	}
}

bool HitachiLCD::lcdMoveCursorDown()
{
	if (IS_ON_FIRST_LINE(cadd)) //primer línea del LCD
	{
		cursorPosition newPos;
		newPos.column = cadd;
		newPos.row = 2;
		lcdSetCursorPosition(newPos);
		return true;
	}
	else if (IS_ON_SECOND_LINE(cadd))  //segunda línea del LCD
	{
		return false; //no se puede mover hacia abajo
	}
	else   //el cadd esta fuera de rango
	{
		return false;
	}
}

bool HitachiLCD::lcdMoveCursorRight()
{
	bool ret = true;
	cadd++;
	if (cadd == END_OF_FIRST_LINE + 1)		
	{
		cadd = BEGIN_OF_SECOND_LINE;		
	}
	if (cadd = END_OF_SECOND_LINE + 1)		//Si estaba en el final de la segunda fila
	{
		cadd = END_OF_SECOND_LINE;		//Lo deja ahi y devuelve false indicando que no pudo moverlo
		ret = false;
	}
	lcdUpdateCursor();
	return ret;
}

bool HitachiLCD::lcdMoveCursorLeft()
{
	bool ret = true;
	if (cadd == BEGIN_OF_FIRST_LINE)	//si esta en el principio de la primer linea
	{
		ret=false;		//no puede moverlo
	}
	else if (cadd = BEGIN_OF_SECOND_LINE)	
	{
		cadd = END_OF_FIRST_LINE;
	}
	else
	{
		cadd--;
	}
	lcdUpdateCursor();
	return ret;
}

bool HitachiLCD::lcdSetCursorPosition(const cursorPosition pos)
{
	bool ret = true;
	switch (pos.row)
	{
	case 1:
		{
			cadd = pos.column;
			break;
		}
		
	case 2:
		{
			cadd = pos.column + (LINES_GAP + CANT_COL);
			break;
		}
	default:
		{
			ret = false;
		}
	}
	if (pos.column > MAX_COL || pos.column < MIN_COL)
	{
		ret = false;
	}
	lcdUpdateCursor();
	return ret;
}

cursorPosition HitachiLCD::lcdGetCursorPosition()
{
	cursorPosition currPos;
	if (IS_ON_FIRST_LINE(cadd))
	{
		currPos.row = 1;
		currPos.column = cadd;
	}
	else if (IS_ON_SECOND_LINE(cadd))
	{
		currPos.column = cadd - (LINES_GAP + CANT_COL);
		currPos.row = 2;
	}
	else
	{
		currPos.column = -1;
		currPos.row = -1;
	}
	return currPos;
}

void HitachiLCD::lcdUpdateCursor()
{
	lcdWriteIR(device_handler, LCD_SET_DDRAM_ADRESS | Hcadd());
}

unsigned char HitachiLCD::Hcadd()
{
	char hCadd = 0;
	if (IS_ON_FIRST_LINE(cadd) || IS_ON_SECOND_LINE(cadd)) //primer o segunda línea del LCD
	{
		hCadd = ((char)cadd) - 1;
	}
	else   //el cadd esta fuera del hitachiLCD
	{
		hCadd = (char) ERROR_HCADD;
	}
	return hCadd;
}
