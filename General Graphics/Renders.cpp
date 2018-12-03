#include "Renders.h"

//////////////////////////////////////////////////////
//////////                           /////////////////
//////////             CLOCK         /////////////////
//////////                           /////////////////
//////////////////////////////////////////////////////

Clock::Clock(pos c, uint8_t r)
{
	rad = r;
	center = c;
	clk_p = pos(c.x, c.y - r);
}

void Clock::clock_Move(int dir)
{
	int top_line = (center.y - rad);
	int bottom_line = (center.y + rad);
	int left_column = (center.x - rad);
	int right_column = (center.x + rad);

	if(clk_p.y == top_line)
	{
		clk_p.x+=dir;
		if(clk_p.x == right_column || clk_p.x == left_column)
			clk_p.y = top_line + 1;
	}
	else if(clk_p.x == left_column)
	{
		clk_p.y-=dir;
		if(clk_p.y == top_line || clk_p.y == bottom_line)
			clk_p.x = (left_column) + 1;
	}
	else if(clk_p.y == bottom_line)
	{
		clk_p.x-=dir;
		if(clk_p.x == right_column || clk_p.x == left_column)
			clk_p.y = bottom_line - 1;
	}
	else if(clk_p.x == right_column)
	{
		clk_p.y+=dir;
		if(clk_p.y == top_line || clk_p.y == bottom_line)
			clk_p.x = right_column - 1;
	}
}

void Clock::RENDER()
{
	DrawLine(clk_p.x, clk_p.y, center.x, center.y);
}

//////////////////////////////////////////////////////
//////////                           /////////////////
//////////             SQUARE        /////////////////
//////////                           /////////////////
//////////////////////////////////////////////////////

Square::Square(sqr r)
{
	Positions = r;
}

void Square::RENDER()
{
	pos tl = Positions.TopLeft;
	pos br = Positions.BottomRight;

	pos tr = pos(tl.x + (br.x - tl.x), tl.y);
	pos bl = pos(tl.x, tl.y + (br.y - tl.y));

	DrawLine(tl.x, tl.y, tr.x, tr.y);
	DrawLine(tr.x, tr.y, br.x, br.y);
	DrawLine(br.x, br.y, bl.x, bl.y);
	DrawLine(bl.x, bl.y, tl.x, tl.y);
}

void Square::square_Move(pos p)
{
	Positions.TopLeft.x += p.x; 
	Positions.TopLeft.y += p.y;

	Positions.BottomRight.x += p.x;
	Positions.BottomRight.y += p.y;
}

//////////////////////////////////////////////////////
//////////                           /////////////////
//////////             SQUARE        /////////////////
//////////                           /////////////////
//////////////////////////////////////////////////////

Rectangle::Rectangle(rect r)
{
	Positions = r;
}

void Rectangle::RENDER()
{

	DrawLine(Positions.TopLeft.x, Positions.TopLeft.y, Positions.TopRight.x, Positions.TopRight.y);
	DrawLine(Positions.TopRight.x, Positions.TopRight.y, Positions.BottomRight.x, Positions.BottomRight.y);
	DrawLine(Positions.BottomRight.x, Positions.BottomRight.y, Positions.BottomLeft.x, Positions.BottomLeft.y);
	DrawLine(Positions.BottomLeft.x, Positions.BottomLeft.y, Positions.TopLeft.x, Positions.TopLeft.y);
}

//////////////////////////////////////////////////////
//////////                           /////////////////
//////////             CIRCLE        /////////////////
//////////                           /////////////////
//////////////////////////////////////////////////////

Circle::Circle(pos c, uint8_t r)
{
	Center = c;
	rad = r;
}

void Circle::RENDER()
{
	int rr = rad << 1;

	int x = rad-1;
	int y = 0;
	int dx = 1;
	int dy = 1;
	int err = dx - (rr);

	while (x >= y)
	{
		 OLED_SET_PIXEL(Center.x + x, Center.y + y, true);
		 OLED_SET_PIXEL(Center.x + y, Center.y + x, true);
		 OLED_SET_PIXEL(Center.x - y, Center.y + x, true);
		 OLED_SET_PIXEL(Center.x - x, Center.y + y, true);
		 OLED_SET_PIXEL(Center.x - x, Center.y - y, true);
		 OLED_SET_PIXEL(Center.x - y, Center.y - x, true);
		 OLED_SET_PIXEL(Center.x + y, Center.y - x, true);
		 OLED_SET_PIXEL(Center.x + x, Center.y - y, true);

		 if (err <= 0)
		 {
			 y++;
			 err += dy;
			 dy += 2;
		 }
		 
		 if (err > 0)
		 {
			 x--;
			 dx += 2;
			 err += dx - (rr);
		 }
	}
}

//////////////////////////////////////////////////////
//////////                           /////////////////
//////////             WAVE          /////////////////
//////////                           /////////////////
//////////////////////////////////////////////////////

Wave::Wave(pos p, uint8_t l, uint8_t m, uint8_t c, double freq)
{
	this->p = p;
	length = l;
	magnitude = m;
	animation_coefficient = c;
	this->freq = freq;

	counter = 1;
	count_way = 1;
}

void Wave::RENDER()
{
	uint8_t wl = ((p.x + length) < SSD1306_LCDWIDTH)?length:SSD1306_LCDWIDTH;
	uint8_t y = 0;

	for(int i = 0; i < wl; i++)
	{
		y = magnitude*fast_sine((freq*(i+counter) / wl) + (coss?0.25:0));
		OLED_SET_PIXEL(i+p.x, y+p.y, true);
	}

	counter+=count_way;
	if(counter > animation_coefficient)
	{
		count_way = -1;
	}
	else if(counter <= 1)
	{
		count_way = 1;
	}
}