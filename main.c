#include "includesNdefines.h"
#include "oled_iic_controll.h"

uint8_t dummy = 1;

int main(void)
{
	DDRC = 0xFF;
	PORTC = 0;

	DDRA = 0xFF;
	PORTA = 0;

	OLED_INIT();

	dummy = 0b11;
    while (1) 
	{	
		dummy ++;
		OLED_SEND_DAT(dummy);
		//OLED_SEND_CMD(SSD1306_DISPLAYON);
    }
}

