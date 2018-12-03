/* 
* Block.cpp
*
* Created: 17.11.2018 14:21:41
* Author: 1
*/


#include "Block.h"

// default constructor
Block::Block()
{
} //Block

Block::Block(pos ps, pos sz, blockTypes bt)
{
	this->ps = ps;
	block_sz = sz;
	status_reg = 0;
	type = bt;

	int w = sz.x;
	int h = sz.y;

	if(bt == I)							//oooo
	{
		b[0] = pos(ps.x + 0, ps.y + 0);
		b[1] = pos(ps.x + w, ps.y + 0);
		b[2] = pos(ps.x + w<<1, ps.y + 0);
		b[3] = pos(ps.x + w*3, ps.y + 0);
		status_reg |= (2<<B_CenterAddr00);
	}
	else if(bt == L)					// ooo
	{									//	 o
		b[0] =  pos(ps.x + 0, ps.y + 0);
		b[1] =  pos(ps.x + w, ps.y + 0);
		b[2] =  pos(ps.x + (w<<1), ps.y + 0);
		b[3] =  pos(ps.x + (w<<1), ps.y + h);
		status_reg |= (2<<B_CenterAddr00);
	}
	else if(bt == rL)					// ooo
	{									//	 o
		b[0] =  pos(ps.x + 0, ps.y + 0);
		b[1] =  pos(ps.x + w, ps.y + 0);
		b[2] =  pos(ps.x + (w<<1), ps.y + 0);
		b[3] =  pos(ps.x + (w<<1), ps.y - h);
		status_reg |= (2<<B_CenterAddr00);
	}
	else if(bt == s)					//	oo
	{									//	oo
		b[0] =  pos(ps.x + 0, ps.y + 0);
		b[1] =  pos(ps.x + w, ps.y + 0);
		b[2] =  pos(ps.x + 0, ps.y + h);
		b[3] =  pos(ps.x + h, ps.y + h);
	}
	else if(bt == Z)					//	oo
	{									//	 oo
		b[0] =  pos(ps.x + 0, ps.y + 0);
		b[1] =  pos(ps.x + w, ps.y + 0);
		b[2] =  pos(ps.x + w, ps.y + h);
		b[3] =  pos(ps.x + (w<<1), ps.y + h);
		status_reg |= (1<<B_CenterAddr00);
	}
	else if(bt == rZ)					//	oo
	{									// oo
		b[0] =  pos(ps.x + 0, ps.y + h);
		b[1] =  pos(ps.x + w, ps.y + h);
		b[2] =  pos(ps.x + w, ps.y + 0);
		b[3] =  pos(ps.x + (w<<1), ps.y + 0);

		status_reg |= (1<<B_CenterAddr00);
	}
	else if(bt == p)					//	 o
	{									//	ooo
		b[0] =  pos(ps.x + 0, ps.y + h);
		b[1] =  pos(ps.x + w, ps.y + h);
		b[2] =  pos(ps.x + w, ps.y + 0);
		b[3] =  pos(ps.x + (w<<1), ps.y + h);

		status_reg |= (1<<B_CenterAddr00);
	}

	Flip();
}

void Block::DrawBlock(Sprite s)
{
	for(int i = 0; i < 4; i++)
	{
		s.DisplaySpriteOnPosititon(b[i]);
	}
}

void Block::MoveSideways(int dir)
{
	int inc = (dir>0?block_sz.y:(-block_sz.y));

	for(int i = 0; i < 4; i++)
	{
		if((dir > 0) && ((b[i].y + inc) > (SSD1306_LCDHEIGHT - inc)))
		{
			status_reg |= (1<<B_NoSidewaysL);
			break;
		}
		else if((dir < 0) && ((b[i].y + (inc<<1)) < inc))
		{
			status_reg |= (1<<B_NoSidewaysR);
			break;
		}
	}

	if(	((!(status_reg & (1<<B_NoSidewaysR))) && (inc < 0))
		||
		((!(status_reg & (1<<B_NoSidewaysL))) && (inc > 0)))
	{
		for(int i = 0; i < 4; i++)
		{
			b[i] = pos(b[i].x, b[i].y + inc);
		}
		ps = pos(ps.x, ps.y + inc);
	}

	status_reg &= ~((1<<B_NoSidewaysL)|(1<<B_NoSidewaysR));
}

void Block::SetPos(pos pss)
{
	ps = pss;

	int minw = 255, minh = 255;
	for(int i = 0; i < 4; i++)
	{
		int buf =b[i].x/block_sz.x; 
		if(buf < minw)
			minw = buf;

		buf =b[i].y/block_sz.y;
		if(buf < minh)
			minh = buf;
	}

	for(int i = 0; i < 4; i++)
	{
		int buf = b[i].x/block_sz.x;
		int x = buf - minw;

		buf = b[i].y/block_sz.y;
		int y = buf - minh;

		b[i] = pos(pss.x + x*block_sz.x, pss.y +  y*block_sz.y);
	}
}

void Block::MoveDown()
{
	for(int i = 0; i < 4; i++)
	{
		if((b[i].x + 1) > (SSD1306_LCDWIDTH-block_sz.x))
		{
			status_reg |= (1<<B_Stopped);
			break;
		}
	}

	if(!(status_reg & (1<<B_Stopped)))
	{
		for(int i = 0; i < 4; i++)
		{
			b[i]=pos(b[i].x + 1, b[i].y);
		}
		ps=pos(ps.x + 1, ps.y);
		steps++;
	}
}

void Block::Flip()
{
	int ind = ((status_reg >> B_CenterAddr00) & (3));
	pos center = b[ind];

	if(type != Z && type != rZ && type != s)
	{
		for(int i = 0; i < 4; i++)
		{
			int sign = 1;
			if(b[i].y != center.y)
				sign = -1;

			b[i] = ps + pos((center.y - b[i].y) * sign, (center.x - b[i].x) * sign);
		}
	}
	else if(type != s)
	{
		for(int i = 0; i < 4; i++)
		{
			int sign = 1;
			if(b[i].y != center.y)
				sign = 1;

			b[i] = ps + pos((center.y - b[i].y) * sign, (center.x - b[i].x) * sign);
		}
	}
}

// default destructor
Block::~Block()
{
} //~Block


