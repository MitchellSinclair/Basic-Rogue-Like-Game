/*
 * parMon.h
 *
 *  Created on: Oct 28, 2018
 *      Author: mitchellsinclair
 */

#ifndef PARMON_H_
#define PARMON_H_


#include <iostream>
#include "monster.h"
#include "Dungeon.h"

class parMon{
	int roll(std::string die);
public:
	parMon();
	~parMon();
	monster gen(dungeon dun);

	std::string description;
	std::string name;
	std::string color;
	std::string HPArr;
	std::string DamArr;
	std::string speedArr;
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
	bool found;
	int HPbase;
	int HPdice;
	int HPsides;
	int Dambase;
	int Damdice;
	int Damsides;
	int Speedbase;
	int Speeddice;
	int Speedsides;
};


#endif /* PARMON_H_ */
