#ifndef __BLOCK_H__
#define __BLOCK_H__

#include "../SpriteGameEngine/GameEngine2D.h"

#define B_Stopped 0
#define B_NoSidewaysR 1
#define B_NoSidewaysL 2
#define B_CenterAddr00 3
#define B_CenterAddr01 4

class Block
{
//variables
public:
	enum blockTypes{Z, rZ, L, rL, I, s, p};
	blockTypes type;
	uint8_t status_reg;
	pos b[4];
	pos ps;
	pos block_sz;
	int steps;

//functions
public:
	Block();
	Block(pos position, pos size, blockTypes type);
	void DrawBlock(Sprite s);
	void MoveSideways(int);
	void SetPos(pos);
	void MoveDown();
	void Flip();
	~Block();
protected:
}; //Block

#endif //__BLOCK_H__
