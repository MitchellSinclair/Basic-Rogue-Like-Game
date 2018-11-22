/*
 * Parser.cpp
 *
 *  Created on: Oct 28, 2018
 *      Author: mitchellsinclair
 */
#include "Parser.h"
#include "parMon.h"
#include "parItm.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <stdint.h>

Parser::Parser() {
}

Parser::~Parser() {
}

std::vector<parMon>  Parser::parseMons() {
	char *homel = strdup(getenv("HOME"));
	char *pathl = (char *) malloc(sizeof(homel) + 17);
	strcpy(pathl, homel);
	strcat(pathl, "/.rlg327/monster_desc.txt");
	std::ifstream file(pathl);
	if(!file){
		std::cout << "file not found" << std::endl;
	}

	std::string s;
	std::string header;
	file >> s;
	header += s + " ";
	file >> s;
	header += s + " ";
	file >> s;
	header += s + " ";
	file >> s;
	header += s;

	std::vector<parMon> monLst;

	if (!header.compare("RLG327 MONSTER DESCRIPTION 1\n")) {
		std::cout << "Not a Valid File" << std::endl;
		std::cout << header << std::endl;
		return monLst; 
	}

	while (file >> s) {
		if (s.compare("BEGIN")==0) {
			//std::cout<<s<<std::endl;
			file >> s;
			if (s.compare("MONSTER")==0) {
				//std::cout<<s<<std::endl;
				file >> s;
				bool found[9];
				for(int i = 0; i < 9; ++i){
					found[i] = false;
				}
				bool error = false;
				parMon mon;
				//std::cout<<s<<std::endl;
				while (s.compare("END")!=0) {
					//std::cout<<s<<std::endl;
					if (s.compare("SYMB")==0) {
						if (found[0]) {
							error = true;
						}
						file>>s;
						mon.symb = s.back();
						found[0] = true;
					} else if (s.compare("COLOR")==0) {
						if (found[1]) {
							error = true;
						}
						std::string abil;
						getline(file, abil);
						std::string word;
						std::istringstream line(abil);
						while(line>>word){
							mon.color= mon.color + word + " ";
						}
						found[1] = true;
					} else if (s.compare("SPEED")==0) {
						if (found[2]) {
							error = true;
						}
						file>>s;
						mon.speedArr = s;
						int indP = 0;
						while(mon.speedArr.at(indP) != '+'){
							indP++;
						}
						int indD = indP + 1;
						while(mon.speedArr.at(indD) != 'd'){
							indD++;
						}
						mon.Speedbase = std::stoi(mon.speedArr.substr(0,indP));
						mon.Speeddice = std::stoi(mon.speedArr.substr(indP+1,indD));
						mon.Speedsides = std::stoi(mon.speedArr.substr(indD+1));
						found[2] = true;
					} else if (s.compare("HP")==0) {
						if (found[3]) {
							error = true;
						}
						file>>s;
						mon.HPArr = s;
						int indP = 0;
						while(mon.HPArr.at(indP) != '+'){
							indP++;
						}
						int indD = indP + 1;
						while(mon.HPArr.at(indD) != 'd'){
							indD++;
						}
						mon.HPbase = std::stoi(mon.HPArr.substr(0,indP));
						mon.HPdice = std::stoi(mon.HPArr.substr(indP+1,indD));
						mon.HPsides = std::stoi(mon.HPArr.substr(indD+1));
						found[3] = true;
					} else if (s.compare("DAM")==0) {
						if (found[4]) {
							error = true;
						}
						file>>s;
						mon.DamArr = s;
						int indP = 0;
						while(mon.DamArr.at(indP) != '+'){
							indP++;
						}
						int indD = indP + 1;
						while(mon.DamArr.at(indD) != 'd'){
							indD++;
						}
						mon.Dambase = std::stoi(mon.DamArr.substr(0,indP));
						mon.Damdice = std::stoi(mon.DamArr.substr(indP+1,indD));
						mon.Damsides = std::stoi(mon.DamArr.substr(indD+1));
						found[4] = true;
					} else if (s.compare("RRTY")==0) {
						if (found[5]) {
							error = true;
						}
						file>>s;
						mon.rarity = std::stoi(s);
						found[5] = true;
					} else if (s.compare("ABIL")==0) {
						if (found[6]) {
							error = true;
						}

						std::string abil;
						getline(file, abil);
						std::string word;
						std::istringstream line(abil);
						while (line >> word) {
							//std::cout<<word<<std::endl;
							if (word.compare("SMART")==0) {
								mon.Intelligence = true;
							} else if (word.compare("TELE")==0) {
								mon.Telepathy = true;
							} else if (word.compare("TUNNEL")==0) {
								mon.Tunneling = true;
							} else if (word.compare("ERRATIC")==0) {
								mon.Erratic = true;
							} else if (word.compare("PASS")==0) {
								mon.Pass = true;
							} else if (word.compare("PICKUP")==0) {
								mon.Pickup = true;
							} else if (word.compare("DESTROY")==0) {
								mon.Destroy = true;
							} else if (word.compare("UNIQ")==0) {
								mon.Unique = true;
							} else if (word.compare("BOSS")==0) {
								mon.Boss = true;
							}
						}
						found[6] = true;
					} else if (s.compare("NAME")==0) {
						if (found[7]) {
							error = true;
						}
						getline(file, mon.name);
						mon.name = mon.name.substr(1, mon.name.length()-1);
						found[7] = true;
					} else if (s.compare("DESC")==0) {
						if (found[8]) {
							error = true;
						}
						std::string desc = "";
						std::string line;
						while (true) {
							getline(file, line);
							if (line.compare(".")==0) {
								desc = desc.substr(0, desc.size()-1);
								mon.description = desc;
								found[8] = true;
								break;
							}
							desc += line+"\n";
						}
					}
					file >> s;
				}
				for (int i = 0; i < 9; ++i) {
					if (!found[i]) {
						std::cout<<"Field not Finished: "<<i<<std::endl;
						error = true;
					}
				}
				if (error) {
					std::cout<<"Error Found"<<std::endl;
					continue;
				}
				monLst.push_back(mon);
			}
		}
	}

	free(pathl);
	free(homel);
	return monLst;
}

std::vector<parItm> Parser::parseItms(){
char *homel = strdup(getenv("HOME"));
	char *pathl = (char *) malloc(sizeof(homel) + 17);
	strcpy(pathl, homel);
	strcat(pathl, "/.rlg327/object_desc.txt");
	std::ifstream file(pathl);
	if(!file){
		std::cout << "file not found" << std::endl;
	}

	std::string s;
	std::string header;
	file >> s;
	header += s + " ";
	file >> s;
	header += s + " ";
	file >> s;
	header += s + " ";
	file >> s;
	header += s;

	std::vector<parItm> monLst;

	if (!header.compare("RLG327 OBJECT DESCRIPTION 1\n")) {
		std::cout << "Not a Valid File" << std::endl;
		std::cout << header << std::endl;
		return monLst; 
	}

	while (file >> s) {
		if (s.compare("BEGIN")==0) {
			//std::cout<<s<<std::endl;
			file >> s;
			if (s.compare("OBJECT")==0) {
				//std::cout<<s<<std::endl;
				file >> s;
				bool found[14];
				for(int i = 0; i < 14; ++i){
					found[i] = false;
				}
				bool error = false;
				parItm itm;
				//std::cout<<s<<std::endl;
				while (s.compare("END")!=0) {
					//std::cout<<s<<std::endl;
					if (s.compare("COLOR")==0) {
						if (found[1]) {
							error = true;
						}
						std::string abil;
						getline(file, abil);
						std::string word;
						std::istringstream line(abil);
						while(line>>word){
							itm.color= itm.color + word + " ";
						}
						found[1] = true;
					} else if (s.compare("SPEED")==0) {
						if (found[2]) {
							error = true;
						}
						file>>s;
						itm.speed = s;
						found[2] = true;
					} else if (s.compare("TYPE")==0){
						if (found[3]) {
							error = true;
						}
						file>>s;
						itm.type = s;
						found[3] = true;
					} else if (s.compare("DAM")==0) {
						if (found[4]) {
							error = true;
						}
						file>>s;
						itm.damage = s;
						found[4] = true;
					} else if (s.compare("RRTY")==0) {
						if (found[5]) {
							error = true;
						}
						file>>s;
						itm.rarity = std::stoi(s);
						found[5] = true;
					} else if (s.compare("HIT")==0) {
						if (found[6]) {
							error = true;
						}
						file>>s;
						itm.hit = s;
						found[6] = true;
					} else if (s.compare("NAME")==0) {
						if (found[7]) {
							error = true;
						}
						getline(file, itm.name);
						itm.name = itm.name.substr(1, itm.name.length()-1);
						found[7] = true;
					} else if (s.compare("DESC")==0) {
						if (found[8]) {
							error = true;
						}
						std::string desc = "";
						std::string line;
						while (true) {
							getline(file, line);
							if (line.compare(".")==0) {
								desc = desc.substr(0, desc.size()-1);
								itm.description = desc;
								found[8] = true;
								break;
							}
							desc += line+"\n";
						}
					} else if (s.compare("DODGE")==0) {
						if (found[9]) {
							error = true;
						}
						file>>s;
						itm.dodge = s;
						found[9] = true;
					} else if (s.compare("DEF")==0) {
						if (found[0]) {
							error = true;
						}
						file>>s;
						itm.defense = s;
						found[0] = true;
					} else if (s.compare("WEIGHT")==0) {
						if (found[10]) {
							error = true;
						}
						file>>s;
						itm.weight = s;
						found[10] = true;
					} else if (s.compare("ATTR")==0) {
						if (found[11]) {
							error = true;
						}
						file>>s;
						itm.attr = s;
						found[11] = true;
					} else if (s.compare("VAL")==0) {
						if (found[12]) {
							error = true;
						}
						file>>s;
						itm.value = s;
						found[12] = true;
					} else if (s.compare("ART")==0) {
						if (found[13]) {
							error = true;
						}
						file>>s;
						itm.artifact = s;
						found[13] = true;
					}
					file >> s;
				}
				for (int i = 0; i < 14; ++i) {
					if (!found[i]) {
						std::cout<<"Field not Finished: "<<i<<std::endl;
						error = true;
					}
				}
				if (error) {
					std::cout<<"Error Found"<<std::endl;
					continue;
				}
				monLst.push_back(itm);
			}
		}
	}

	free(pathl);
	free(homel);
	return monLst;
}
