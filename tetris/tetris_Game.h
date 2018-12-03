#ifndef __TETRIS_GAME_H__
#define __TETRIS_GAME_H__

#include "Block.h"
#include <stdlib.h>

#define		BLOCK_SIZE	4
#define		ROW_SIZE	(SSD1306_LCDHEIGHT/(BLOCK_SIZE*8))
#define		FIELD_SIZE	(SSD1306_LCDWIDTH/BLOCK_SIZE) * ROW_SIZE

#define bLeft 2
#define bFlip 1
#define bRight 0

#define IsPressed 7
#define IsHold 6
#define CntToPressed 0x06
#define SidewaysCntToPressed 0x03

class tetris_Game
{

//variables
public:
	int		GameOver;
	int		score;
private:
	int		field_pos[FIELD_SIZE] = {0};
	Block	Active_block;
	Sprite	block;
	int		current_index;

//functions
public:
	tetris_Game();
	void GameIteration(int buttons[3]);
	~tetris_Game();
private:
	void CheckPosAndDraw();
	int CheckPos();
	void EraseFullBlocks();

}; //tetris_Game

#endif 
