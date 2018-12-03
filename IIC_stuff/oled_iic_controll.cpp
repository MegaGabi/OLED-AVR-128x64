#include "oled_iic_controll.h"

//----------------------------------------------
//------------------OLED_SEND_CMDS----------------
//----------------------------------------------


void IIC_init(unsigned long frq)
{
	TWBR = (F_CPU/(2*frq))-8;//set scl frequency
	TWSR = 0;
}

void IIC_start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	//_delay_us(2);
}

void IIC_stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
	while ((TWCR & (1<<TWSTO)));
}

void IIC_write(uint8_t dat)
{
	TWDR = dat;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
	//_delay_us(2);
}

unsigned char IIC_read(uint8_t ack)
{
	TWCR = ack ?
	((1 << TWINT) | (1 << TWEN) | (1 << TWEA))
	: ((1 << TWINT) | (1 << TWEN)) ;
	while (!(TWCR & (1<<TWINT)));
	return TWDR;

}

void IIC_scan()
{
	for(uint8_t i = 0; i <= 127; i++)
	{
		IIC_start();
		IIC_write(SLA_W(i));
		if(TWSR == 0x18)
		{
			//TODO:do something useful on address found
			PORTA = i;
			break;
		}
		IIC_stop();
	}
}

uint8_t IIC_Read_ack(void)
{
	
	// start TWI module and acknowledge data after reception
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

uint8_t IIC_Read_nack(void)
{
	
	// start receiving without acknowledging reception
	TWCR = (1<<TWINT) | (1<<TWEN);
	// wait for end of transmission
	while( !(TWCR & (1<<TWINT)) );
	// return received data from TWDR
	return TWDR;
}

//----------------------------------------------
//----------------OLED_SEND_CMDs-----------------
//----------------------------------------------

//OLED_data

uint8_t PAGES[SSD1306_LCDHEIGHT/8][SSD1306_LCDWIDTH];

void OLED_INIT()
{
	IIC_init(400000UL);

	OLED_SEND_CMD(SSD1306_DISPLAYOFF);
	OLED_SEND_CMD(SSD1306_SETDISPLAYCLOCKDIV);
	OLED_SEND_CMD(0x80);
	OLED_SEND_CMD(SSD1306_SETMULTIPLEX);
	OLED_SEND_CMD(64-1);//height-1
	OLED_SEND_CMD(SSD1306_SETDISPLAYOFFSET);
	OLED_SEND_CMD(0x00);//no offset
	OLED_SEND_CMD(SSD1306_SETSTARTLINE| 0x00);//set start line
	OLED_SEND_CMD(SSD1306_CHARGEPUMP);//charge pump
	OLED_SEND_CMD(0x14);//dclk upto 14-15
	OLED_SEND_CMD(SSD1306_MEMORYMODE);
	OLED_SEND_CMD(0x00);//OLED RAM
	OLED_SEND_CMD(SSD1306_SEGREMAP| 0x01);
	OLED_SEND_CMD(SSD1306_COMSCANDEC);
	OLED_SEND_CMD(SSD1306_SETCOMPINS);
	OLED_SEND_CMD(0x12);
	OLED_SEND_CMD(SSD1306_SETCONTRAST);
	OLED_SEND_CMD(0xcf);
	OLED_SEND_CMD(SSD1306_SETPRECHARGE);
	OLED_SEND_CMD(0xf1);
	OLED_SEND_CMD(SSD1306_SETVCOMDETECT);
	OLED_SEND_CMD(0x40);
	OLED_SEND_CMD(SSD1306_DISPLAYALLON_RESUME);
	OLED_SEND_CMD(SSD1306_NORMALDISPLAY);
	OLED_SEND_CMD(SSD1306_DISPLAYON);
	OLED_SEND_CMD(SSD1306_COLUMNADDR);
	OLED_SEND_CMD(0x00);
	OLED_SEND_CMD(0x7f);
	OLED_SEND_CMD(SSD1306_PAGEADDR);
	OLED_SEND_CMD(0x00);
	OLED_SEND_CMD(0x07);
}

void OLED_SEND_CMD(uint8_t cmd)
{
	IIC_start();
	IIC_write(SLA_W(0x3C));
	IIC_write(START_CMD_TRANSMISSION);
	IIC_write(cmd);
	IIC_stop();
}

void OLED_SEND_DAT(uint8_t dat)
{
	IIC_start();
	IIC_write(SLA_W(0x3C));
	IIC_write(START_DAT_TRANSMISSION);
	IIC_write(dat);
	IIC_stop();
}

void OLED_CLS()
{
	uint16_t h = sizeof(PAGES);

	memset(PAGES, 0, h);
}

void OLED_RENDER()
{
	IIC_start();
	IIC_write(SLA_W(0x3C));
	IIC_write(START_DAT_TRANSMISSION);
	for(int i = 0; i < 8; i++)
	{
		for(int j = 0; j < 128; j++)
		{
			IIC_write(PAGES[i][j]);
		}
	}
	IIC_stop();
}

void OLED_SET_COLUMN_REG(uint8_t x, uint8_t y, int reg, bool state)
{
	if(x >= SSD1306_LCDWIDTH)
		return;
	if(y >= SSD1306_LCDHEIGHT)
		return;

	int res_y = (y>>3);			//y/8
	
	int bit_y = 0, toPut = 0;

	if(res_y < (SSD1306_LCDHEIGHT/8))
	{
		bit_y = (y & (0b111));	//y - int(y/8)

		toPut = (reg << (bit_y));

		PAGES[res_y][x] = (state? (PAGES[res_y][x] | toPut) : (PAGES[res_y][x] & ~(toPut)));
	}
	else 
		return;

	if(res_y < (SSD1306_LCDHEIGHT/8) - 1)
	{
		bit_y = ((~bit_y & (0b111)))+1;	//8 - (y - int(y/8))

		toPut = (reg >> bit_y);

		PAGES[res_y+1][x] = state? PAGES[res_y+1][x] | toPut : PAGES[res_y+1][x] & ~(toPut);
	}

}

int OLED_GET_PIXEL(uint8_t x, uint8_t y)
{
	if(x >= SSD1306_LCDWIDTH)
		return 0;
	if(y >= SSD1306_LCDHEIGHT)
		return 0;

	int res_y = (y>>3); //y/8
	int bit_y = (y & (0b111)); //y - int(y/8)
		
	return ((PAGES[res_y][x] & (1<<bit_y))?1:0);
}

void OLED_SET_PIXEL(uint8_t x, uint8_t y, bool state)
{
	if(x >= SSD1306_LCDWIDTH)
		return;
	if(y >= SSD1306_LCDHEIGHT)
		return;

	int res_y = (y>>3); //y/8
	int bit_y = (y & (0b111)); //y - int(y/8)
	
	PAGES[res_y][x] = state? PAGES[res_y][x] | (1<<bit_y) : PAGES[res_y][x] & ~(1<<bit_y);
}


