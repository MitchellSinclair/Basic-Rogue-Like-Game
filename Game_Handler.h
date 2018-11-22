/*
 * Game_Handler.h
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#ifndef GAME_HANDLER_H_
#define GAME_HANDLER_H_

#include "Dungeon.h"
#include "monster.h"
#include "parMon.h"
#include "parItm.h"

class Game_Handler{
private:
	int numberOfMon;
	bool PCAlive;

	void generateMonsters(std::vector<parMon> monLst, std::vector<parItm> itmLst);
public:
	int levelNum;
	dungeon level;
	monster *levelMons;
	const char *s;
	int *moveQ;
	monster *monLst;
	std::vector<parMon> mon; 
	std::vector<parItm> itm;

	Game_Handler(bool load, int monNum, std::vector<parMon> monList, std::vector<parItm> itmLst);
	~Game_Handler();
	void movePlayer(int c);
	void moveMon(int min);
	int checkEnd();
	void teleport();
};



#endif /* GAME_HANDLER_H_ */
