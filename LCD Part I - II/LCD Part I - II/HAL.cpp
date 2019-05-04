#include "HAL.h"


#define DELAY_1MS	1ms
#define DELAY_3MS	3ms
#define DELAY_5MS	5ms
#define DELAY_10MS	10ms



using namespace std;


//DECLARACION FUNCIONES AUXILIARES


//Escribe el Nibble menos significativo del byte que recibe.		//listo
void lcdWriteNybble(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char register_select);

//Escribe el byte que recibe en el registro indicado.				//listo
void lcdWriteByte(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char rs);


//listo -verificar si hay que hacer el delay dentro de nybble tambien, capaz lo estas haciendo dos veces y eso puede provocar que no se inicialize ya que no se respeta los tiempos pedidos-
FT_HANDLE * lcdInit(int num_device)
{

	/*
			Inicialización 
			
			Cuando comenzamos a usar el display debemos asegurarnos de que esté inicializado. 
			
			Para inicializar en el modo de 4 bits debemos seguir esta secuencia: 
			
			1. Enviar el nibble alto de “function set” con el modo en 8 bits 
			2. Esperar 5 ms. 
			3. Enviar el nibble alto de “function set” con el modo en 8 bits 
			4. Esperar 1ms. 
			5. Enviar el nibble alto de “function set” con el modo en 8 bits 
			6. Esperar 1ms. 
			7. Enviar el nibble alto de “function set” con el modo en 4 bits. 
			(a partir de ahora enviamos las instrucciones separadas en dos nibbles) 
			8. Esperar 1ms. 
			9. Enviar la instrucción “function set” con el modo en 4 bits, 2 líneas y fuente de 5x8 
			10. Esperar 1 ms. 
			11. Enviar la instrucción “display on/off control” con todo apagado 
			12. Esperar 1 ms. 
			13. Enviar la instrucción “clear screen” 
			14. Esperar 10ms. 
			15. Enviar la instrucción “entry mode set” 

			
			-sacado de la guia, pagina 9 de 22 -
	
	
	*/



	FT_STATUS status = !FT_OK;
	FT_HANDLE* deviceHandler = (FT_HANDLE*) new (FT_HANDLE);

	chrono::seconds max_time_to_connect(CONNECTING_TIME);							//le doy un tiempo para conectarse al display, definido por el define "CONNECTING_TIME" en segundos

	chrono::time_point<chrono::system_clock> start = chrono::system_clock::now();   //empiezo a cronometrar
	chrono::time_point<chrono::system_clock> current = start;

	while ((status != FT_OK) && ((current - start) < max_time_to_connect))			//busco conectarme al display
	{
		status = FT_OpenEx((void *) WHICH_LCD, num_device, deviceHandler);

		if (status == FT_OK)
		{


			UCHAR Mask = MASK_ALL_BITS_BYTE;												//Que pines serán considerados como  outputs: utilizaremos todos los pines (la mascara a pasar será  0xFF).
			UCHAR Mode = LCD_SETBITMODE_ASYNCHRONOUS; 												//Modo de transmisión de datos: se utilizará el modo asincrónico (mode 1).

																							//anteriores dos lineas, se realizaron siguiendo la filmina 11 de "Presentacion FTDI"
			
			
			if (FT_SetBitMode(*deviceHandler, Mask, Mode) == FT_OK)
			{
			/*	
					LSB -> DL
					B1	-> 1 (para decir que estamos en la instruccion que define el modo de datos a enviar)
					demas bits en 0

					El bit DL controla el modo de datos(1 para 8 bits, 0 para 4 bits).
					• En el modo de 8 bits, los datos se envían directamente como ya vimos.
					• En el modo de 4 bits, sólo se usan 4 líneas(D4 - D7).Las líneas D0 - D3 están
					deshabilitadas.Para enviar un byte es necesario separarlo en 2 nibbles.El nibble alto
					(bit 7 - bit 4) se envía primero, y luego se continúa con el nibble bajo(bit 3 - bit 0).

			*/



				lcdWriteNybble(deviceHandler, (LCD_FUNCTION_SET| LCD_FUNCTION_SET_DL_8BITS) >> 4 , SET_IR_ON); //paso 1

				this_thread::sleep_for(DELAY_5MS);																 //paso 2
				
				lcdWriteNybble(deviceHandler, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_DL_8BITS) >> 4, SET_IR_ON); //paso 3

				this_thread::sleep_for(DELAY_1MS);																 //paso 4
				
				lcdWriteNybble(deviceHandler, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_DL_8BITS) >> 4, SET_IR_ON); //paso 5
				
				this_thread::sleep_for(DELAY_1MS);																 //paso 6
				
				lcdWriteNybble(deviceHandler, (LCD_FUNCTION_SET | LCD_FUNCTION_SET_DL_4BITS) >> 4, SET_IR_ON); //paso 7
				
																												 //incializado el modo 4 bits.
				
				this_thread::sleep_for(DELAY_1MS);																 //paso 8


				UCHAR data = (LCD_FUNCTION_SET | LCD_FUNCTION_SET_DL_4BITS | LCD_FUNCTION_SET_N_2_LINES | LCD_FUNCTION_SET_FONT_5x8); 

				// instrucción “function set” con el modo en 4 bits, 2 líneas y fuente de 5x8

				lcdWriteNybble(deviceHandler, data >> 4, SET_IR_ON);											//paso 9 a							
				lcdWriteNybble(deviceHandler, data, SET_IR_ON);													//paso 9 b

				this_thread::sleep_for(DELAY_1MS);																//paso 10

				data = (LCD_DISPLAY_CONTROL | LCD_DISPLAY_CONTROL_DISPLAY_OFF | LCD_DISPLAY_CONTROL_CURSOR_OFF | LCD_DISPLAY_CONTROL_BLINK_OFF);
				
				//instrucción “display on/off control” con todo apagado

				lcdWriteNybble(deviceHandler, data >> 4, SET_IR_ON);											//paso 11 a							
				lcdWriteNybble(deviceHandler, data, SET_IR_ON);													//paso 11 b

				this_thread::sleep_for(DELAY_1MS);																//paso 12

				lcdWriteByte(deviceHandler, LCD_CLEAR, SET_IR_ON);												//paso 13
				
				this_thread::sleep_for(DELAY_10MS);																//paso 14
				
				lcdWriteByte(deviceHandler, LCD_ENTRY_MODE_SET, SET_IR_ON);										//paso 15

			}
			else
			{
				FT_Close(deviceHandler);
				delete deviceHandler;
				return nullptr;
			}

		}

		current = chrono::system_clock::now();
	}

	if (status != FT_OK) //sali del loop, ¿me pude conectar o se acabo el tiempo?
	{
		//se acabo el tiempo

		delete deviceHandler;
		return nullptr;
	}

	return deviceHandler;
}


//listo
void lcdWriteIR(FT_HANDLE * deviceHandler, unsigned char value)
{
	lcdWriteByte(deviceHandler, value, SET_IR_ON);

}//listo

//listo
void lcdWriteDR(FT_HANDLE * deviceHandler, unsigned char value)
{
	lcdWriteByte(deviceHandler, value, SET_DR_ON);
}



//DEFINICION FUNCIONES AUXILIARES

//mando la parte baja de "unsigned char byte"		//listo
void lcdWriteNybble(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char register_select)
{
	/*
		Por lo tanto debemos seguir los siguientes pasos para hacer una escritura en el display:
		1. Asegurarnos de que E vale 0
		2. Configurar RS en función del registro que queramos acceder. El valor 0 para el IR o el
		valor 1 para el DR
		3. Poner el dato o comando en el bus de datos
		4. Poner E en 1, esperar el tiempo que el comando requiera, y luego poner E en 0

			-sacado de la guia, pagina 3 de 22 (al final)-

	*/


	DWORD cantbytes_written2device = 0;
	unsigned char data;
	data = ((byte << 4) & MASK_HIGH_PART_BYTE); //me quedo con los 4 bits bajos del byte y los shifteo 4 veces


	data = ((data | (register_select)) & (SET_ENABLE_OFF));
	FT_Write((*deviceHandler), (LPVOID)&data, sizeof(data), &cantbytes_written2device);
	this_thread::sleep_for(DELAY_1MS);

	if (register_select == SET_DR_ON) //no me interesa saber cuanto escribio si es data
	{
		cantbytes_written2device = 0;
	}

	data = (data | (SET_ENABLE_ON));
	FT_Write(*deviceHandler, (LPVOID)&data, sizeof(data), &cantbytes_written2device);
	this_thread::sleep_for(DELAY_3MS);


	if (register_select == SET_DR_ON) //no me interesa saber cuanto escribio si es data
	{
		cantbytes_written2device = 0;
	}

	data = data & (SET_ENABLE_OFF);
	FT_Write(*deviceHandler, (LPVOID)&data, sizeof(data), &cantbytes_written2device);
	this_thread::sleep_for(DELAY_1MS);



}


//listo
void lcdWriteByte(FT_HANDLE * deviceHandler, unsigned char byte, unsigned char register_select)
{
	lcdWriteNybble(deviceHandler, (byte >> 4), register_select); //escribe nybble mas significativo.
	lcdWriteNybble(deviceHandler, byte, register_select); //escribe nybble menos significativo.
} //listo
