#ifndef OLED_IIC_CONTROLL_H_
#define OLED_IIC_CONTROLL_H_

#include "includesNdefines.h"

#define SLA_W(address)  (address << 1)
#define SLA_R(address)  ((address << 1) + 0x01)

//control byte
#define START_CMD_TRANSMISSION	0x80
#define START_DAT_TRANSMISSION	0x40			

//trash
#define SSD1306_LCDWIDTH                  128
#define SSD1306_LCDHEIGHT                 64

//OLED SSD1306 commands as given in datasheet of ssd 1306
#define SSD1306_SETCONTRAST 0x81
#define SSD1306_DISPLAYALLON_RESUME 0xA4
#define SSD1306_DISPLAYALLON 0xA5
#define SSD1306_NORMALDISPLAY 0xA6
#define SSD1306_INVERTDISPLAY 0xA7
#define SSD1306_DISPLAYOFF 0xAE
#define SSD1306_DISPLAYON 0xAF

#define SSD1306_SETDISPLAYOFFSET 0xD3
#define SSD1306_SETCOMPINS 0xDA

#define SSD1306_SETVCOMDETECT 0xDB

#define SSD1306_SETDISPLAYCLOCKDIV 0xD5
#define SSD1306_SETPRECHARGE 0xD9

#define SSD1306_SETMULTIPLEX 0xA8

#define SSD1306_SETLOWCOLUMN 0x00
#define SSD1306_SETHIGHCOLUMN 0x10

#define SSD1306_SETSTARTLINE 0x40

#define SSD1306_MEMORYMODE 0x20
#define SSD1306_COLUMNADDR 0x21
#define SSD1306_PAGEADDR   0x22

#define SSD1306_COMSCANINC 0xC0
#define SSD1306_COMSCANDEC 0xC8

#define SSD1306_SEGREMAP 0xA0

#define SSD1306_CHARGEPUMP 0x8D

#define SSD1306_EXTERNALVCC 0x1
#define SSD1306_SWITCHCAPVCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A


//iic_func
			void	IIC_init(unsigned long);
			void	IIC_start();
			void	IIC_stop();
			void	IIC_write(uint8_t);
unsigned	char	IIC_read(uint8_t);
			void	IIC_scan();
			uint8_t IIC_Read_nack(void);
			uint8_t IIC_Read_ack(void);

//OLED_func

			void	OLED_INIT();
			void	OLED_SEND_CMD(uint8_t);
			void	OLED_SEND_DAT(uint8_t dat);
			void	OLED_CLS();
			void	OLED_RENDER();
			int		OLED_GET_PIXEL(uint8_t x, uint8_t y);
			void	OLED_SET_PIXEL(uint8_t x, uint8_t y, bool state);
			void	OLED_SET_COLUMN_REG(uint8_t x, uint8_t y, int reg, bool state);

#endif /* OLED_IIC_CONTROLL_H_ */