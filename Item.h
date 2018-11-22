#ifndef ITEM_H_
#define ITEM_H_

#include <iostream>
#include <string>

class Item {
public:

	std::string description;
	std::string name;
	int color;
	std::string type;
	int hit;
	std::string damage;
	int dodge;
	int defense;
	int speed;
	int attr;
	int value;
	bool artifact;
	int weight;
	int rarity;
	int X;
	int Y;
	bool picked;

	Item();
	~Item();
};

#endif /* ITEM_H_ */
