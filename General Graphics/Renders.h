/*
 * Renders.h
 *
 * Created: 09.11.2018 17:07:04
 *  Author: 1
 */ 


#ifndef RENDERS_H_
#define RENDERS_H_

#include "../IIC_stuff/includesNdefines.h"
#include "../IIC_stuff/oled_iic_controll.h"
#include "NeatGrapics.h"


struct pos
{
	int x;
	int y;

	pos()
	{}

	pos(int x, int y)
	{
		this->x = x;
		this->y = y;
	}

	int operator==(pos p)
	{
		return ((p.x == this->x) && (p.y == this->y));
	}

	pos operator-(pos p)
	{
		return pos((this->x - p.x), (this->y - p.y));
	}

	pos operator+(pos p)
	{
		return pos((this->x + p.x), (this->y + p.y));
	}

	void operator+=(pos p)
	{
		this->x += p.x;
		this->y += p.y;
	}

	void operator-=(pos p)
	{
		this->x -= p.x;
		this->y -= p.y;
	}

	pos operator<<(int p)
	{
		return pos((this->x << p), (this->y << p));
	}

	pos operator>>(int p)
	{
		return pos((this->x >> p), (this->y >> p));
	}

	pos operator*(int p)
	{
		return pos((this->x * p), (this->y * p));
	}
};

struct pos3d
{
	double x, y, z;
};

struct sqr
{
	pos TopLeft;
	pos BottomRight;
	
	sqr()
	{}

	sqr(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
	{
		this->TopLeft.x = x1;
		this->TopLeft.y = y1;
		
		this->BottomRight.x = x2;
		this->BottomRight.y = y2;
	}
};

struct rect
{
	pos TopLeft;
	pos TopRight;
	pos BottomRight;
	pos BottomLeft;

	rect()
	{}

	rect(pos tl, pos tr, pos br, pos bl)
	{
		TopLeft = tl;
		TopRight = tr;
		BottomRight = br;
		BottomLeft = bl;
	}
};

class Renderable
{
	public: virtual void RENDER() = 0;
};

class Clock: public Renderable
{
	pos clk_p;//позиция наконечника стрелки

	uint8_t rad;//радиус часов
	pos center;//центра часов

	public: Clock(){}
	public: Clock(pos, uint8_t);
	public: void clock_Move(int dir);
	public: void RENDER();
};

class Square: public Renderable
{
	sqr	Positions;	//позиции квадарата 
	uint8_t fill;		//закрашивать или нет

	public: Square(sqr);
	public: void RENDER();
	public: void square_Move(pos);
};

class Rectangle: public Renderable
{
	rect	Positions;	//позиции квадарата
	uint8_t fill;		//закрашивать или нет

	public: Rectangle(rect);
	public: void RENDER();
};

class Circle: public Renderable
{
	public: pos Center;
	uint8_t rad;

	public: Circle(pos, uint8_t);
	public: void RENDER();
};

class Wave: public Renderable
{
	pos		p;	//позиция графика
	uint8_t length; 
	uint8_t magnitude;
	uint8_t animation_coefficient;
	public: double freq;
	public: bool coss;

	uint8_t counter;
	int count_way;

	public: Wave(){};
	public: Wave(pos startPos, uint8_t l, uint8_t m, uint8_t c, double freq);
	public: void RENDER();
};
#endif /* RENDERS_H_ */