#include "tetris_Game.h"

// default constructor
tetris_Game::tetris_Game()
{
	GameOver = 0;
	//for(int i = 0; i < FIELD_SIZE; i++)
	//{
		//if(i >= 32)
		//{
			//if(i%ROW_SIZE)
				//field_pos[i] = 0xFF;
			//else
				//field_pos[i] = 0x7F;
		//}
	//}
	block = Sprite(BLOCK);
	Active_block = Block(pos(0, 28), block.sz, Block::I);
	Active_block.Flip();
} //tetris_Game



void tetris_Game::CheckPosAndDraw()
{
	//	FIELD
	//	o--------------------------------o
	//	|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
	//	|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
	//	o--------------------------------o
	for(int i = 0; i < FIELD_SIZE; i++)
	{
		int y = (i/ROW_SIZE)*BLOCK_SIZE;

		//	ROW
		//	o--------------------------------o
		//	|	  X  						 |
		//	|	  X  						 |
		//	o--------------------------------o

		int r = i%ROW_SIZE;

		//	ROW PART(COLUMNS)
		//	o--------------------------------o
		//	|	  X 						 |
		//	|	     						 |
		//	o--------------------------------o
		for(int j = 0; j < 8; j++)
		{
			if((field_pos[i]>>j) & 1)
			{
				int x = BLOCK_SIZE*(j+(r<<3));

				pos toDraw = pos(y, x);
				pos toCheckD = pos(y - block.sz.x, x);
				pos toCheckR = pos(y - block.sz.x, x + block.sz.y);
				pos toCheckL = pos(y - block.sz.x, x - block.sz.y);

				for(int bl = 0; bl < 4; bl++)
				{
					pos ABpos = pos(Active_block.b[bl].x - (Active_block.b[bl].x%BLOCK_SIZE), Active_block.b[bl].y);

					if(ABpos == toCheckD)
					{
						Active_block.status_reg |= (1<<B_Stopped);
					}
					if(ABpos == toCheckR)
					{
						Active_block.status_reg |= (1<<B_NoSidewaysR);
					}
					if(ABpos == toCheckL)
					{
						Active_block.status_reg |= (1<<B_NoSidewaysL);
					}
				}

				block.DisplaySpriteOnPosititon(toDraw);
			}
		}
	}
}

int tetris_Game::CheckPos()
{
	//	FIELD
	//	o--------------------------------o
	//	|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
	//	|XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX|
	//	o--------------------------------o
	for(int i = 0; i < FIELD_SIZE; i++)
	{
		int y = (i/ROW_SIZE)*BLOCK_SIZE;

		//	ROW
		//	o--------------------------------o
		//	|	  X  						 |
		//	|	  X  						 |
		//	o--------------------------------o

		int r = i%ROW_SIZE;

		//	ROW PART(COLUMNS)
		//	o--------------------------------o
		//	|	  X 						 |
		//	|	     						 |
		//	o--------------------------------o
		for(int j = 0; j < 8; j++)
		{
			if((field_pos[i]>>j) & 1)
			{
				int x = BLOCK_SIZE*(j+(r<<3));

				pos toDraw = pos(y, x);

				for(int bl = 0; bl < 4; bl++)
				{
					pos ABpos = pos(Active_block.b[bl].x - (Active_block.b[bl].x%BLOCK_SIZE), Active_block.b[bl].y);

					if(ABpos == toDraw)
					{
						return 1;
					}
				}
			}
		}
	}
	return 0;
}

void tetris_Game::EraseFullBlocks()
{
	int full_row = 1;

	for(int i = (FIELD_SIZE - ROW_SIZE), r = 0; i >= 0;)
	{
		full_row = 1;
		for(int j = i; j < (i+ROW_SIZE); j++)
		{
			if(r!=0)
			{
				int ind = j-(r*ROW_SIZE);
				if(ind >= 0)
					field_pos[j] = field_pos[ind];
				else
					field_pos[j] = 0;
			}

			if(field_pos[j] != 0xFF)
			{
				full_row = 0;
			}
		}

		if(full_row)
		{
			r++;
			score++;
		}
		else
			i-=ROW_SIZE;
	}
}

void tetris_Game::GameIteration(int buttons[3])
{
	CheckPosAndDraw();

	if(Active_block.status_reg & (1<<B_Stopped))
	{
		for (int i = 0; i < 4; i++)
		{
			int x = (Active_block.b[i].y / BLOCK_SIZE) % 8;
			int y = (Active_block.b[i].x / BLOCK_SIZE) * ROW_SIZE + (Active_block.b[i].y >= 32);

			field_pos[y] |= (1<<x);
		}

		EraseFullBlocks();

		if(Active_block.steps == 0)
			GameOver = 1;

		Active_block = Block(pos(0, BLOCK_SIZE*8), block.sz, (Block::blockTypes)(rand()%(Block::p+1)));
	}
	else 
	{
		Active_block.MoveDown();

		if((buttons[bLeft] & (1<<IsPressed)) && ((buttons[bLeft]&0x3F) > SidewaysCntToPressed))
		{
			Active_block.MoveSideways(1);

			buttons[bLeft] &= (1<<IsPressed|1<<IsHold);
		}

		if((buttons[bRight] & (1<<IsPressed)) && ((buttons[bRight]&0x3F) > SidewaysCntToPressed))
		{
			Active_block.MoveSideways(-1);

			buttons[bRight] &= (1<<IsPressed|1<<IsHold);
		}

		if(	(buttons[bFlip] & (1<<IsPressed)) 
			&&
			(!(buttons[bFlip] & (1<<IsHold))) )
		{
			if(buttons[bFlip] & (1<<IsPressed))
			{
				buttons[bFlip] |= (1<<IsHold);
			}

			Block buf = Active_block;

			Active_block.Flip();
			if(CheckPos())
			{
				Active_block = buf;
			}
		}

		Active_block.DrawBlock(block);
	}
}

// default destructor
tetris_Game::~tetris_Game()
{
} //~tetris_Game
