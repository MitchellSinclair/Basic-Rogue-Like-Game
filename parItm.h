/*
 * parMon.h
 *
 *  Created on: Oct 28, 2018
 *      Author: mitchellsinclair
 */

#ifndef PARITM_H_
#define PARITM_H_


#include <iostream>
#include "Item.h"

class parItm{
	int roll(std::string die);
public:
	parItm();
	~parItm();
	Item gen();

	std::string description;
	std::string name;
	std::string color;
	std::string type;
	std::string hit;
	std::string damage;
	std::string dodge;
	std::string defense;
	std::string speed;
	std::string attr;
	std::string value;
	std::string artifact;
	std::string weight;
	int rarity;
	bool found;
};


#endif /* PARITM_H_ */
