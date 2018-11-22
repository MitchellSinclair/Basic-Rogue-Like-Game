/*
 * monster.h
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

class dungeon;

#ifndef MONSTER_H_
#define MONSTER_H_

#include <iostream>
#include <string>
#include "Dungeon.h"

class monster {
public:

	char symb;
	int rarity;
	bool Intelligence;
	bool Telepathy;
	bool Tunneling;
	bool Erratic;
	bool Pass;
	bool Pickup;
	bool Destroy;
	bool Unique;
	bool Boss;
	int num;
	int X;
	int Y;
	int exists;
	int speed;
	int color;
	bool found;
	int HP;
	int DamageBase;
	int DamageSides;
	int DamageNum;
	const char* name;
	const char* desc;

	monster(dungeon dun);
	monster();
	~monster();
	int canSee(dungeon dun);
	int findMove(dungeon dun);
	void getLet();
};

#endif /* MONSTER_H_ */
