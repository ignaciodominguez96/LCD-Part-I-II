#include "HitachiLCD.h"

#define BEGIN_FIRST_LINE 1			
#define END_FIRST_LINE 16		
#define BEGIN_SECOND_LINE 65		
#define END_SECOND_LINE 80		
#define MIN_COL 1
#define MAX_COL 16
#define LINES_GAP (BEGIN_SECOND_LINE-END_FIRST_LINE)
#define ON_FIRST_LINE(a) (((a >= BEGIN_FIRST_LINE) && (a <= END_FIRST_LINE)))
#define ON_SECOND_LINE(a) (((a >= BEGIN_SECOND_LINE) && (a <= END_SECOND_LINE)))


//listo
HitachiLCD::HitachiLCD(void)
{


	this->can_init = false;
	this->error = true;
	device_handler = lcdInit();
	if (device_handler != nullptr)
	{
		this->can_init = true; //Se inicializo bien
		this->error = false;
	}
	
	
	cadd = BEGIN_FIRST_LINE;
}

//listo
HitachiLCD:: ~HitachiLCD()
{
	if (this->can_init == true)
	{
		FT_Close(*device_handler);
		delete device_handler;
	}

}

//listo
bool HitachiLCD::lcdInitOk()
{
	return this->can_init;
}


//listo
FT_STATUS HitachiLCD::lcdGetError()
{
	FT_STATUS state = FT_GetStatus(*device_handler, nullptr, nullptr, nullptr);
	if (state != FT_OK)
	{
		this->error = true;
	}
	return state;
}


//listo
bool HitachiLCD::lcdClear()
{
	bool can_clear_display = false;

	FT_STATUS state = FT_OK;
	
	lcdWriteIR(device_handler, LCD_CLEAR);
	
	if (state == FT_OK)
	{
		this->cadd = BEGIN_FIRST_LINE;
		can_clear_display = true;
	}
	else
	{
		this->error = true;
	}
	return can_clear_display;
}


//listo
bool HitachiLCD::lcdClearToEOL()
{
	bool can_clear_to_eol = false;
	int cadd_aux = cadd;
	int limit = 0;
	if (ON_FIRST_LINE(cadd))
	{
		limit = END_FIRST_LINE;
	}
	else if (ON_SECOND_LINE(cadd))
	{
		limit = END_SECOND_LINE;
	}
	else
	{
		std::cout << "ERROR: Cursor Overflow" << std::endl;
		this->error = true;
	}



	if (this->error == false)
	{
		can_clear_to_eol = true;

		for (; cadd <= limit ; cadd++)
		{
			lcdWriteDR(device_handler, ' '); //imprimo espacios, simulando que borro el display, letra por letra.
		}

		cadd = cadd_aux; //regreso el cursor a la posición que tenia antes de ejecutar la función.
		lcdUpdateCursor();
	}

	return can_clear_to_eol;  //devuelve true si hubo error, false si no hubo error

}


//listo
basicLCD& HitachiLCD::operator<<(const char c)
{
	lcdWriteDR((this->device_handler), c);
	cadd++;

	if (cadd == (END_FIRST_LINE + 1))
	{
		cadd = BEGIN_SECOND_LINE;
	}
	if (cadd == (END_SECOND_LINE + 1))
	{
		cadd = BEGIN_FIRST_LINE;
	}

	lcdUpdateCursor();
	return *this;
};


//listo
basicLCD& HitachiLCD::operator<<(const char * c)
{
	for (int i = 0; c[i] != '\0'; i++)
	{
		lcdWriteDR((this->device_handler), c[i]);
		cadd++;
		
		if (cadd == (END_FIRST_LINE + 1))
		{
			cadd = BEGIN_SECOND_LINE;
		}
		if (cadd == (END_SECOND_LINE + 1))
		{
			cadd = BEGIN_FIRST_LINE;
		}



		lcdUpdateCursor();
	}
	return *this;
};


//listo
basicLCD& HitachiLCD::operator<<(std::string str)
{
	for (unsigned int i = 0; i < str.size(); i++)
	{
		lcdWriteDR((this->device_handler), str[i]);
		
		
		cadd++;
		
		if (cadd == (END_FIRST_LINE + 1))
		{
			cadd = BEGIN_SECOND_LINE;
		}
		if (cadd == (END_SECOND_LINE + 1))
		{
			cadd = BEGIN_FIRST_LINE;
		}

		lcdUpdateCursor();
	}

	return *this;
};


//listo
bool HitachiLCD::lcdMoveCursorUp()
{
	if (ON_FIRST_LINE(cadd)) //primer línea del LCD
	{
		return false; //no se puede mover hacia arriba
	}
	else if (ON_SECOND_LINE(cadd))  //segunda línea del LCD
	{
		cursorPosition newPos;
		newPos.row = 1;
		newPos.col = cadd - (LINES_GAP + MAX_COL) + 1;
		lcdSetCursorPosition(newPos);
		return true;
	}
	else   //el cadd esta fuera de rango
	{
		return false;
	}
}


//listo
bool HitachiLCD::lcdMoveCursorDown()
{
	if (ON_FIRST_LINE(cadd)) //primer línea del LCD
	{
		cursorPosition newPos;
		newPos.col = cadd - 1;
		newPos.row = 2;
		lcdSetCursorPosition(newPos);
		return true;
	}
	else if (ON_SECOND_LINE(cadd))  //segunda línea del LCD
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

	if (cadd == (END_FIRST_LINE + 1))		
	{
		cadd = BEGIN_SECOND_LINE;		
	}
	if (cadd == (END_SECOND_LINE + 1))		//Si estaba en el final de la segunda fila
	{
		cadd = END_SECOND_LINE;		//Lo deja ahi y devuelve false indicando que no pudo moverlo
		ret = false;
	}
	lcdUpdateCursor();
	return ret;
}


bool HitachiLCD::lcdMoveCursorLeft()
{
	bool ret = true;
	if (cadd == BEGIN_FIRST_LINE)	//si esta en el principio de la primer linea
	{
		ret=false;		//Lo deja ahi y devuelve false indicando que no pudo moverlo
	}
	else if (cadd == (BEGIN_SECOND_LINE))	
	{
		cadd = END_FIRST_LINE;
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
			cadd = pos.col;
			break;
		}
		
		case 2:
		{
			cadd = pos.col + (LINES_GAP + MAX_COL);
			break;
		}
		default:
		{
			ret = false;
		}
	}
	if (pos.col > MAX_COL || pos.col < MIN_COL)
	{
		ret = false;
	}
	lcdUpdateCursor();
	return ret;
}

cursorPosition HitachiLCD::lcdGetCursorPosition()
{
	cursorPosition pos;

	if (ON_FIRST_LINE(cadd))
	{
		pos.row = 1;
		pos.col = cadd;
	}
	else if (ON_SECOND_LINE(cadd))
	{
		pos.col = cadd - (LINES_GAP + MAX_COL);
		pos.row = 2;
	}
	else
	{
		pos.col = -1;
		pos.row = -1;
	}
	return pos;
}

void HitachiLCD::lcdUpdateCursor()
{
	lcdWriteIR(device_handler, LCD_SET_DDRAM_ADRESS | cadd_cmd());
}




unsigned char HitachiLCD::cadd_cmd()
{
	char temp_cadd = 0;
	if (ON_FIRST_LINE(cadd) || ON_SECOND_LINE(cadd)) //primer o segunda línea del LCD
	{
		temp_cadd = ((char)cadd) - 1;
	}
	else   //el cursor esta fuera del LCD
	{
		temp_cadd = (char) 0xFF;
	}
	return temp_cadd;
}
