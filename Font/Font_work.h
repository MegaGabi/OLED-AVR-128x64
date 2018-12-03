
#ifndef __FONT_WORK_H__
#define __FONT_WORK_H__

#include <avr/pgmspace.h>

#include "../IIC_stuff/includesNdefines.h"
#include "../General Graphics/Renders.h"
#include "../IIC_stuff/oled_iic_controll.h"

const int zero[] PROGMEM = {252, 2, 129, 97, 25, 5, 2, 252, 1, 2, 5, 4, 4, 4, 2, 1};
const int one[] PROGMEM = {0, 0, 0, 2, 255, 0, 0, 0, 0, 0, 0, 4, 7, 4, 0, 0};
const int two[] PROGMEM = {0, 12, 18, 129, 65, 34, 28, 0, 0, 6, 5, 4, 4, 4, 4, 4};
const int three[] PROGMEM = {0, 2, 1, 1, 1, 34, 220, 0, 0, 2, 4, 4, 4, 2, 1, 0};
const int four[] PROGMEM = {0, 0, 112, 76, 66, 255, 64, 0, 0, 0, 0, 0, 0, 7, 0, 0};
const int five[] PROGMEM = {0, 31, 17, 9, 9, 9, 17, 224, 0, 1, 2, 4, 4, 4, 2, 1};
const int six[] PROGMEM = {252, 34, 17, 17, 17, 33, 194, 0, 1, 2, 4, 4, 4, 2, 1, 0};
const int seven[] PROGMEM = {0, 7, 130, 130, 193, 185, 135, 0, 0, 0, 0, 0, 7, 0, 0, 0};
const int eight[] PROGMEM = {0, 222, 33, 33, 33, 222, 0, 0, 0, 3, 4, 4, 4, 3, 0, 0};
const int nine[] PROGMEM = {0, 30, 33, 33, 33, 254, 0, 0, 0, 2, 4, 4, 4, 3, 0, 0};

const int A[] PROGMEM = {1};
const int B[] PROGMEM = {1};
const int C[] PROGMEM = {1};

const int* const DigitsPointers[] PROGMEM = {zero, one, two, three, four, five, six, seven, eight, nine};
const int* const CharsPointers[] PROGMEM = {A, B, C};

class Font_work
{
public:
	Font_work();
	static void DisplayDigit(pos point, unsigned int digit);
	static void DisplayNumber(pos point, unsigned int number);
	~Font_work();
protected:
private:
	Font_work( const Font_work &c );
	Font_work& operator=( const Font_work &c );

};

#endif //__FONT_WORK_H__
