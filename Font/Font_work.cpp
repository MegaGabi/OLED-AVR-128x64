
#include "Font_work.h"

// default constructor
Font_work::Font_work()
{
} //Font_work

void Font_work::DisplayDigit(pos p, unsigned int digit)
{
	if(digit > 9)
		digit = 9;

	int* point = (int*)pgm_read_word(&(DigitsPointers[digit]));

	for(int i = 0; i < 16; i++)
	{
		int dig = (int)pgm_read_word(&(point[i]));

		OLED_SET_COLUMN_REG(p.x+(i>=8?(i-8):i), p.y+(i>=8?8:0), dig, true);
	}
}

void Font_work::DisplayNumber(pos point, unsigned int number)
{
	unsigned int l = 0;
	unsigned int cnt = number;
	while(cnt != 0)
	{
		cnt /= 10;
		l++;
	}

	int i = 0;
	while(number != 0)
	{	
		DisplayDigit(pos(point.x + ((l-i)*8), point.y), number%10);
		number /= 10;
		i++;
	}
}

// default destructor
Font_work::~Font_work()
{
} //~Font_work
