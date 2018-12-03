

#include "GameEngine2D.h"

GameEngine2D::GameEngine2D()
{
}

Sprite::Sprite()
{
	Inverse = false;
}

Sprite::Sprite(pos p, SpriteNames sprite)
{
	(*this) = Sprite(sprite);
	this->ps = p;
}

Sprite::Sprite(SpriteNames sn)
{
	Inverse = false;
	sprite_index = (int)sn;
	ps = pos(0, 0);
	GetSize();
}

void Sprite::GetSize()
{
	int* point = (int*)pgm_read_word(&(Sprites[sprite_index]));
	int size = (int)pgm_read_word(&(point[0]));
	int cols = (int)pgm_read_word(&(point[1]));
	int rows = size/cols;

	sz.x = cols;

	int maxy = 0;
	int y_pos = 0;
	for(int i = (rows-1)*cols; i < size; i++)
	{
		int col = (int)pgm_read_word(&(point[i+2]));

		for(int j = 0; j < 8; j++)
		{
			int bb = col & (1<<(j));

			if(bb && maxy < bb)
			{
				maxy = bb;
				y_pos = j+1;
			}
		}
	}

	sz.y = (rows-1)+y_pos;
}

void Sprite::SetInverse()
{
	Inverse = !Inverse;
}

void Sprite::DisplaySprite()
{
	int* point = (int*)pgm_read_word(&(Sprites[sprite_index]));
	int size = (int)pgm_read_word(&(point[0]));
	int cols = (int)pgm_read_word(&(point[1]));
	int incx = 0, incy = 0;

	for(int i = 0; i < size; i++)
	{
		int col = (int)pgm_read_word(&(point[i+2]));

		incx = i%cols;
		if(incx == 0 && i != 0)
		incy+=8;
		if(Inverse)
			col = ~col;
		OLED_SET_COLUMN_REG(ps.x+(incx), ps.y+(incy), col, true);
	}
}

void Sprite::DisplaySpriteOnPosititon(pos p)
{
	int* point = (int*)pgm_read_word(&(Sprites[sprite_index]));
	int size = (int)pgm_read_word(&(point[0]));
	int cols = (int)pgm_read_word(&(point[1]));
	int incx = 0, incy = 0;

	for(int i = 0; i < size; i++)
	{
		int col = (int)pgm_read_word(&(point[i+2]));

		incx = i%cols;
		if(incx == 0 && i != 0)
		incy+=8;
		if(Inverse)
		col = ~col;
		OLED_SET_COLUMN_REG(p.x+(incx), p.y+(incy), col, true);
	}
}

void Sprite::SetPos(pos p)
{
	this->ps = p;
}

GameEngine2D::~GameEngine2D()
{
}
