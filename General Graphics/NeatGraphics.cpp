//--------------------------------------------------------
//--------------------------------------------------------
//------------------OLED NEATEST GRAPHICS-----------------
//--------------------------------------------------------
//--------------------------------------------------------

#include "NeatGrapics.h"
#include <math.h>
#include <stdlib.h>

void swap(uint8_t& x, uint8_t& y)
{
	int temp = x;
	x = y;
	y = temp;
}

double fast_sine(double x)
{
	int sign = 1;
	if(x < 0)
	{
		x = abs(x);
		sign = -1;
	}

	if(x >= 1)
	{
		x = x - (int)x;
	}

	return (21*x*(x-0.5)*(x-1))*sign;
}

void CheckSize(uint8_t& chk, uint8_t max)
{
	chk = ((chk < max)? chk : max);
}

void DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
	CheckSize(x1, SSD1306_LCDWIDTH);
	CheckSize(x2, SSD1306_LCDWIDTH);

	CheckSize(y1, SSD1306_LCDHEIGHT);
	CheckSize(y2, SSD1306_LCDHEIGHT);

	int dx = x2 - x1;
	int dy = y2 - y1;

	if(abs(dy) < abs(dx))
	{
		if(x1 > x2)
		{
			swap(x1, x2);
			swap(y1, y2);

			dx = x2 - x1;
			dy = y2 - y1;
		}

		int yi = 1;
		if (dy < 0)
		{
			yi = -1;
			dy = -dy;
		}
		int D = (dy<<1) - dx;
		int y = y1;

		for (int x = x1; x < x2; x++)
		{
			OLED_SET_PIXEL(x, y, true);
			if (D > 0)
			{
				y += yi;
				D -= dx<<1;
			}

			D += dy<<1;
		}
	}
	else 
	{
		if(y1 > y2)
		{
			swap(x1, x2);
			swap(y1, y2);

			dx = x2 - x1;
			dy = y2 - y1;
		}

		int xi = 1;
		if (dx < 0)
		{
			xi = -1;
			dx = -dx;
		}
		int D = (dx<<1) - dy;
		int x = x1;

		for (int y = y1; y < y2; y++)
		{
			OLED_SET_PIXEL(x, y, true);
			if (D > 0)
			{
				x += xi;
				D -= dy<<1;
			}
			D += dx<<1;
		}
	}
}