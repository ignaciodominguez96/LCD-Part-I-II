#include "HitachiLCD.h"

#define ERROR_HCADD 0xFF
#define HITACHI_OFFSET_SECOND_LINE 0x30

#define BEGIN_OF_FIRST_LINE 1
#define END_OF_FIRST_LINE 16
#define BEGIN_OF_SECOND_LINE 17
#define END_OF_SECOND_LINE 32
#define FIRST_LINE_RANGE	((cadd >= BEGIN_OF_FIRST_LINE) && (cadd <= END_OF_FIRST_LINE))
#define SECOND_LINE_RANGE	((cadd >= BEGIN_OF_SECOND_LINE) && (cadd <= END_OF_SECOND_LINE))
#define TOTAL_RANGE			((cadd >= BEGIN_OF_FIRST_LINE) && (cadd <= END_OF_SECOND_LINE))
#define IS_ON_FIST_LINE(x) (((x >= BEGIN_OF_FIRST_LINE) && (x <= END_OF_FIRST_LINE)))
#define IS_ON_SECOND_LINE(x) (((x >= BEGIN_OF_SECOND_LINE) && (x <= END_OF_SECOND_LINE)))
#define MIN_ROW 0
#define MAX_ROW 1
#define MIN_COL 0
#define MAX_COL 15
#define CANT_ROW	2
#define CANT_COL	16
#define IS_VALID_ROW(x) (((x >= MIN_ROW) && (x <= MAX_ROW)))
#define IS_VALID_COL(x) (((x >= MIN_COL) && (x <= MAX_COL)))
#define IS_VALID_POS(pos) ((IS_VALID_ROW(pos.row) && IS_VALID_COL(pos.column)))

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

bool HitachiLCD::lcdGetError()
{
	bool ret = false;
	DWORD * lpdwAmountInRxQueue = nullptr;
	DWORD * lpdwAmountInTxQueue = nullptr;
	DWORD * lpdwEventStatus = nullptr;
	FT_STATUS state = FT_GetStatus(*device_handler, lpdwAmountInRxQueue, lpdwAmountInTxQueue, lpdwEventStatus);
	if (state == FT_OK)
	{
		ret = true;
	}
	else
	{
		error = true;
	}
	return ret;
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
	int cadd_aux = cadd;
	int limit = 0;
	if (IS_ON_FIST_LINE(cadd))
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
	}
	if (!error)
	{
		while (cadd <= limit)
		{
			lcdWriteDR(device_handler, ' '); //imprimo espacios, simulando que borro el display, letra por letra.
			cadd++;
		}
		cadd = cadd_aux; //regreso el cursor a la posición que tenia antes de ejecutar la función.
		lcdUpdateCursor();
	}

	return error;  //devuelve true si hubo error, false si no hubo error

}

basicLCD& HitachiLCD::operator<<(const char c)
{
	lcdWriteDR((this->device_handler), c);
	cadd++;
	if (cadd == (END_OF_SECOND_LINE + 1))
	{
		cadd = BEGIN_OF_FIRST_LINE;
	}
	lcdUpdateCursor();
	return *this;
};

basicLCD& HitachiLCD::operator<<(const char * c)
{
	unsigned int i = 0;
	while (c[i] != '\0')
	{
		lcdWriteDR((this->device_handler), c[i]);
		i++;
		cadd++;
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
	if (FIRST_LINE_RANGE) //primer línea del hitachiLCD
	{
		return false; //no se puede mover hacia arriba
	}
	else if (SECOND_LINE_RANGE)  //segunda línea del hitachiLCD
	{
		cursorPosition newPos;
		newPos.column = 1;
		newPos.row = cadd - CANT_COL;
		lcdSetCursorPosition(newPos);
		return true;
	}
	else   //el cadd esta fuera del hitachiLCD
	{
		return false;
	}
}

bool HitachiLCD::lcdMoveCursorDown()
{
	if (FIRST_LINE_RANGE) //primer línea del hitachiLCD
	{
		cursorPosition newPos;
		newPos.column = cadd;
		newPos.row = 2;
		lcdSetCursorPosition(newPos);
		return true;
	}
	else if (SECOND_LINE_RANGE)  //segunda línea del hitachiLCD
	{
		return false; //no se puede mover hacia abajo
	}
	else   //el cadd esta fuera del hitachiLCD
	{
		return false;
	}
}

bool HitachiLCD::lcdMoveCursorRight()
{
	if (TOTAL_RANGE && (cadd != END_OF_SECOND_LINE)) //no llegue al final
	{
		cursorPosition newPos;
		if (SECOND_LINE_RANGE)
		{
			newPos.row = 2;
			newPos.column = cadd - CANT_COL + 1;
		}
		else if (FIRST_LINE_RANGE && (cadd != END_OF_FIRST_LINE))
		{
			newPos.row = 1;
			newPos.column = cadd + 1;
		}
		else if (cadd == END_OF_FIRST_LINE)
		{
			newPos.row = 2;
			newPos.column = 1;
		}
		lcdSetCursorPosition(newPos);
		return true;
	}
	else //ya no lo puedo mover
	{
		return false;
	}
}

bool HitachiLCD::lcdMoveCursorLeft()
{
	if (TOTAL_RANGE && (cadd != 1)) //no llegue al principio
	{
		cursorPosition newPos;
		if (SECOND_LINE_RANGE && (cadd != BEGIN_OF_SECOND_LINE))
		{
			newPos.row = 2;
			newPos.column = cadd - CANT_COL - 1;
		}
		else if (FIRST_LINE_RANGE)
		{
			newPos.row = 1;
			newPos.column = cadd - 1;
		}
		else if (cadd == BEGIN_OF_SECOND_LINE)
		{
			newPos.row = 1;
			newPos.column = END_OF_FIRST_LINE;
		}
		lcdSetCursorPosition(newPos);
		return true;
	}
	else //ya no lo puedo mover
	{
		return false;
	}
}

bool HitachiLCD::lcdSetCursorPosition(const cursorPosition pos)
{
	switch (pos.row)
	{
	case 1:
		cadd = pos.column;
		break;
	case 2:
		cadd = pos.column + CANT_COL;
		break;
	}
	lcdUpdateCursor();
	return true;
}

cursorPosition HitachiLCD::lcdGetCursorPosition()
{
	cursorPosition currPos;
	if (FIRST_LINE_RANGE)
	{
		currPos.row = 1;
		currPos.column = cadd;
	}
	else if (SECOND_LINE_RANGE)
	{
		currPos.column = cadd - CANT_COL;
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
	if (FIRST_LINE_RANGE) //primer línea del hitachiLCD
	{
		hCadd = ((char)cadd) - 1;
	}
	else if (SECOND_LINE_RANGE)  //segunda línea del hitachiLCD
	{
		hCadd = ((char)cadd) - 1;
		hCadd += HITACHI_OFFSET_SECOND_LINE;
	}
	else   //el cadd esta fuera del hitachiLCD
	{
		hCadd = (char) ERROR_HCADD;
	}
	return hCadd;
}
