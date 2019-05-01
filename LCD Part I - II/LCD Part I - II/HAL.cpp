#include "HAL.h"

//Escribe el Nibble menos significativo del byte que recibe.
void lcdWriteNibble(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char rs);


//Escribe el byte que recibe en el registro indicado.
void lcdWriteByte(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char rs);


void lcdWriteNibble(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char rs)
{
	DWORD sent = 0;
	unsigned char temp;
	temp = ((byte << 4) & (UCHAR)(0xF0));
	if (rs == SET_IR_ON)
	{
		temp = ((temp | (SET_IR_ON)) & (SET_ENABLE_OFF));
		FT_Write((*deviceHandler), (LPVOID)&temp, sizeof(temp), &sent);
		sleep_for(1ms); //delay de 1 ms
		temp = (temp | (SET_ENABLE_ON));
		FT_Write(*deviceHandler, (LPVOID)&temp, sizeof(temp), &sent);
		sleep_for(3ms);//delay de 3ms.
		temp = temp & (SET_ENABLE_OFF);
		FT_Write(*deviceHandler, (LPVOID)&temp, sizeof(temp), &sent);

	}
	else if (rs == SET_DR_ON)
	{

		temp = (temp | (SET_DR_ON)) & (SET_ENABLE_OFF);
		FT_Write(*deviceHandler, (LPVOID)&temp, sizeof(temp), &sent);
		sleep_for(1ms);//delay de 1 ms
		sent = 0;
		temp = temp | (SET_ENABLE_ON);
		FT_Write(*deviceHandler, (LPVOID)&temp, sizeof(temp), &sent);
		sleep_for(3ms);//delay de 3ms.
		sent = 0;
		temp = temp & (SET_ENABLE_OFF);
		FT_Write(*deviceHandler, (LPVOID)&temp, sizeof(temp), &sent);
		sleep_for(1ms);
	}
}

void lcdWriteByte(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char rs)
{
	lcdWriteNibble(deviceHandler, (byte >> 4), rs); //escribe nybble mas significativo.
	lcdWriteNibble(deviceHandler, byte, rs); //escribe nybble menos significativo.
}

FT_HANDLE * lcdInit(int iDevice)
{
	FT_STATUS status = !FT_OK;
	FT_HANDLE* deviceHandler = (FT_HANDLE*) new (FT_HANDLE);

	std::chrono::seconds MaxTime(CONNECTING_TIME);

	std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
	std::chrono::time_point<std::chrono::system_clock> current = start;

	while ((status != FT_OK) && ((current - start) < MaxTime))
	{
		status = FT_OpenEx((void *)MY_LCD_DESCRIPTION, iDevice, deviceHandler);

		if (status == FT_OK)
		{

			UCHAR Mask = 0xFF;	//Selects all FTDI pins.
			UCHAR Mode = 1; 	// Set asynchronous bit-bang mode
			if (FT_SetBitMode(*deviceHandler, Mask, Mode) == FT_OK)
			{

				//Configuracion inicial a modo 4bit
				lcdWriteNibble(deviceHandler, (UCHAR)0x03, SET_IR_ON);
				sleep_for(4ms); //delay de 4ms.
				lcdWriteNibble(deviceHandler, (UCHAR)0x03, SET_IR_ON);
				sleep_for(1ms); //delay de 100 microsegundos.
				lcdWriteNibble(deviceHandler, (UCHAR)0x03, SET_IR_ON);
				lcdWriteNibble(deviceHandler, (UCHAR)0x02, SET_IR_ON);
				//ya esta incializado el modo 4 bits.

				lcdWriteNibble(deviceHandler, (UCHAR)0x04, SET_IR_ON);
				lcdWriteNibble(deviceHandler, (UCHAR)0x08, SET_IR_ON);
				lcdWriteByte(deviceHandler, 0x0E, SET_IR_ON);
				lcdWriteByte(deviceHandler, LCD_CLEAR, SET_IR_ON);
				lcdWriteByte(deviceHandler, 0x0C, SET_IR_ON);

				cout << "init Ok" << endl;
			}
			else
			{
				cout << "Couldn't configure LCD" << endl;

				FT_Close(deviceHandler);
				delete deviceHandler;
				return nullptr;
			}

		}
		current = std::chrono::system_clock::now();
	}

	if (status != FT_OK)
	{
		std::cout << "Error: No se pudo abrir el LCD" << std::endl;
		delete deviceHandler;
		return nullptr;
	}

	return deviceHandler;
}

void lcdWriteIR(FT_HANDLE * deviceHandler, unsigned char valor)
{
	lcdWriteByte(deviceHandler, valor, SET_IR_ON);
}
void lcdWriteDR(FT_HANDLE * deviceHandler, unsigned char valor)
{
	lcdWriteByte(deviceHandler, valor, SET_DR_ON);
}