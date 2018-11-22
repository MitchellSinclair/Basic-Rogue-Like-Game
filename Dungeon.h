/*
 * Dungeon.h
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#ifndef DUNGEON_H_
#define DUNGEON_H_

class monster;

#include <vector>
#include "monster.h"
#include "Item.h"

class dungeon {

	int checkClearence(int locX, int locY, int dimX, int dimY);


public:
	int roll(std::string die);
	int dun[80][21];
	int hardness[80][21];
	int rooms[4][9];
	int working[80][21];
	int nonTunneling[80][21];
	int Tunneling[80][21];
	int PlayerX;
	int PlayerY;
	int PlayerS;
	int PlayerHP;
	Item weapon;
	Item offhand;
	Item ranged;
	Item armor;
	Item helmet;
	Item cloak;
	Item gloves;
	Item boots;
	Item amulet;
	Item light;
	Item ringA;
	Item ringB;
	Item PlayerCarry[10];
	int CarryNum;
	bool done;
	bool weaponEqu;
	bool offhandEqu;
	bool rangedEqu;
	bool armorEqu;
	bool helmetEqu;
	bool cloakEqu;
	bool glovesEqu;
	bool bootsEqu;
	bool amuletEqu;
	bool lightEqu;
	bool ringAEqu;
	bool ringBEqu;
	int upX;
	int upY;
	int downX;
	int downY;
	bool found[80][21];
	bool toggleFog;
	std::vector<monster> mons;
	std::vector<Item> itms;

	dungeon();
	~dungeon();
	void drawdun();
	void calculateNonTunneling();
	void calculateTunneling();
	int PlayerAttack();

};

#endif /* DUNGEON_H_ */
