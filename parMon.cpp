/*
 * parMon.cpp
 *
 *  Created on: Oct 29, 2018
 *      Author: mitchellsinclair
 */
#include "parMon.h"

parMon::parMon(){
	Intelligence = false;
	Telepathy = false;
	Tunneling = false;
	Erratic = false;
	Pass = false;
	Pickup = false;
	Destroy = false;
	Unique = false;
	Boss = false;
	found = false;
}

parMon::~parMon(){

}

monster parMon::gen(dungeon dun){
	monster mon(dun);
	
	mon.symb = symb;
	mon.rarity = rarity;
	mon.Intelligence = Intelligence;
	mon.Telepathy = Telepathy;
	mon.Tunneling = Tunneling;
	mon.Erratic = Erratic;
	mon.Pass = Pass;
	mon.Pickup = Pickup;
	mon.Destroy = Destroy;
	mon.Unique = Unique;
	mon.Boss = Boss;
	mon.speed = roll(speedArr);
	if(color.compare("BLACK") == 0){
		mon.color = 7;
	} else if(color.compare("RED") == 0){
		mon.color = 1;
	} else if(color.compare("GREEN") == 0){
		mon.color = 2;
	} else if(color.compare("YELLOW") == 0){
		mon.color = 3;
	} else if(color.compare("BLUE") == 0){
		mon.color = 4;
	} else if(color.compare("MAGENTA") == 0){
		mon.color = 5;
	} else if(color.compare("CYAN") == 0){
		mon.color = 6;
	} else if(color.compare("WHITE") == 0){
		mon.color = 7;
	}
	mon.found = false;
	mon.HP = roll(HPArr);
	mon.DamageBase = Dambase;
	mon.DamageSides = Damsides;
	mon.DamageNum = Damdice;
	mon.name = name.c_str();
	mon.desc = description.c_str();
	return mon;
}

int parMon::roll(std::string die){
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


