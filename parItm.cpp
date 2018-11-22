#include "parItm.h"
#include <string>

parItm::parItm(){
	found = false;
}

parItm::~parItm(){

}

Item parItm::gen(){
	Item itm;

	itm.description = description;
	itm.name = name;
	if(color.compare("BLACK") == 0){
		itm.color = 0;
	} else if(color.compare("RED") == 0){
		itm.color = 1;
	} else if(color.compare("GREEN") == 0){
		itm.color = 2;
	} else if(color.compare("YELLOW") == 0){
		itm.color = 3;
	} else if(color.compare("BLUE") == 0){
		itm.color = 4;
	} else if(color.compare("MAGENTA") == 0){
		itm.color = 5;
	} else if(color.compare("CYAN") == 0){
		itm.color = 6;
	} else if(color.compare("WHITE") == 0){
		itm.color = 7;
	}
	itm.type = type;
	itm.damage = damage;
	itm.dodge = roll(dodge);
	itm.defense = roll(defense);
	itm.speed = roll(speed);
	itm.attr = roll(attr);
	itm.value = roll(value);
	if(artifact.compare("FALSE") == 0){
		itm.artifact = false;
	} else{
		itm.artifact = true;
	}
	itm.weight = roll(weight);
	itm.rarity = rarity;

	

	return itm;
}

int parItm::roll(std::string die){
	int indP = 0;
	while(die.at(indP) != '+'){
		indP++;
	}
	int indD = indP + 1;
	while(die.at(indD) != 'd'){
		indD++;
	}
	int min = std::stoi(die.substr(0,indP));
	int dice = std::stoi(die.substr(indP+1,indD));
	int sides = std::stoi(die.substr(indD+1));
	int total = 0;
	for(int i = 0; i < dice; i++){
		total += rand()%sides;
	}
	return min + total;
}
