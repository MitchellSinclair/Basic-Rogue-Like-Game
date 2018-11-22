/*
 * Game_Main.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#include <iostream>
#include "Dungeon.h"
#include <ncurses.h>
#include <stdio.h>
#include "Save-Load.h"
#include <string.h>
#include "monster.h"
#include <unistd.h>
#include <limits.h>
#include <curses.h>
#include "Game_Handler.h"
#include <time.h>
#include "Parser.h"
#include <vector>

int main(int argc, char *argv[]) {
	Parser par;
	std::vector<parMon> monLst = par.parseMons();
	std::vector<parItm> itmLst = par.parseItms();

	char *home = strdup(getenv("HOME"));
	char *path = (char *) malloc(sizeof(home) + 9);
	strcpy(path, home);
	strcat(path, "/.rlg327");
	mkdir(path, 0770);

	initscr();
	//start_color();
	noecho();
	cbreak();
	raw();
	keypad(stdscr, TRUE);
	srand(time(NULL));

	int i = 1;
	int numMon = 10;
	while (i < argc) {
		if (strcmp(argv[i], "--nummon") == 0) {
			numMon = atoi(argv[i + 1]);
			break;
		}
		i++;
	}

	i = 1;
	bool load = false;
	while (i < argc) {
		if (strcmp(argv[i], "--load") == 0) {
			load = true;
			break;
		}
		i++;
	}

	Game_Handler game(load, numMon, monLst, itmLst);
	game.level.calculateNonTunneling();
	game.level.calculateTunneling();

	i = 1;
	while (i < argc) {
		if (strcmp(argv[i], "--save") == 0) {
			save(game.level);
			break;
		}
		i++;
	}
	int j;
	while (1 == 1) {
		for (i = game.level.PlayerX - 2; i <= game.level.PlayerX + 2 && i < 80;
				i++) {
			for (j = game.level.PlayerY - 2;
					j <= game.level.PlayerY + 2 && j < 21; j++) {
				if (game.level.hardness[i][j] > 0) {
					continue;
				}
				game.level.found[i][j] = true;
			}
			if (game.level.hardness[i][j] > 0) {
				continue;
			}
		}
		int num = game.checkEnd();
		if (num == 1) {
			clear();
			printw("You Died (RIP)\n");
			int out = 'n';
			refresh();
			while (out != 'Q') {
				out = getch();
			}
			break;
		}
		if (num == 2) {
			clear();
			printw("You Won (YEET!)\n");
			int out = 'n';
			refresh();
			while (out != 'Q') {
				out = getch();
			}
			break;
		}

		int min = 0;
		for (int i = 1; i < numMon + 1; ++i) {
			if (game.moveQ[i] < game.moveQ[min]) {
				if (i < numMon) {
					if (game.monLst[i].exists == 1) {
						min = i;
					}
				} else {
					min = i;
				}
			}
		}

		if (min == numMon) {
			clear();
			printw(game.s);
			game.level.drawdun();
			refresh();
			int c = getch();
			if (c == 'Q') {
				clear();
				endwin();
				free(home);
				free(path);
				return 0;
			} else if (c == 'm') {
				game.movePlayer(c);
			} else if (c == '<' && game.levelNum == 0
					&& game.level.PlayerX == game.level.upX
					&& game.level.PlayerY == game.level.upY) {
				clear();
				printw("You Left The Dungeon");
				refresh();
				while (c != 'Q') {
					c = getch();
				}
				break;
			} else if (c == 'f') {
				if (game.level.toggleFog) {
					game.level.toggleFog = false;
					game.s = "Toggle Fog On\n";
					continue;
				} else {
					game.level.toggleFog = true;
					game.s = "Toggle Fog Off\n";
					continue;
				}
			} else if(c == 'g'){
				game.teleport();
			} else if(c == 'L'){
				int save = game.level.dun[game.level.PlayerX][game.level.PlayerY];
				game.level.dun[game.level.PlayerX][game.level.PlayerY] = 21;
				clear();
				printw("Select Monster: \n");
				game.level.drawdun();
				refresh();
				int Xpos = game.level.PlayerX;
				int Ypos = game.level.PlayerY;
				while(c != 27){
					c = getch();
					switch (c) {
					case 'k':
						game.level.dun[Xpos][Ypos] = save;
						Ypos--;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'j':
						game.level.dun[Xpos][Ypos] = save;
						Ypos++;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'h':
						game.level.dun[Xpos][Ypos] = save;
						Xpos--;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'l':
						game.level.dun[Xpos][Ypos] = save;
						Xpos++;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'u':
						game.level.dun[Xpos][Ypos] = save;
						Xpos++;
						Ypos--;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'y':
						game.level.dun[Xpos][Ypos] = save;
						Xpos--;
						Ypos--;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'n':
						game.level.dun[Xpos][Ypos] = save;
						Xpos++;
						Ypos++;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 'b':
						game.level.dun[Xpos][Ypos] = save;
						Xpos--;
						Ypos++;
						save = game.level.dun[Xpos][Ypos];
						game.level.dun[Xpos][Ypos] = 21;
						break;
					case 't':
						for(unsigned int i = 0; i < game.level.mons.size(); ++i){
							if(game.level.mons.at(i).X == Xpos && game.level.mons.at(i).Y == Ypos){
								clear();
								printw(game.level.mons.at(i).name);
								printw(" Description: \n");
								printw(game.level.mons.at(i).desc);
								refresh();
								while((c=getch())!=27);
							}
						}
					default:
						break;
					}
					clear();
					printw("Select Monster: \n");
					game.level.drawdun();
					refresh();
				}
				game.level.dun[Xpos][Ypos] = save;
				clear();
				printw("Back to Game: \n");
				game.level.drawdun();
				refresh();
			
			} else if(c == 'i'){
				int pointer = 0;
				while(c != 27){
					clear();
					if(c == KEY_DOWN && pointer < game.level.CarryNum-1){
						pointer++;
					}
					else if(c == KEY_UP && pointer > 0){
						pointer--;
					}
					else if(c == 'd' && game.level.CarryNum > 0){
						if(game.level.CarryNum == 1){
							game.level.working[game.level.PlayerX][game.level.PlayerY] = -3;
							game.level.PlayerCarry[0].X = game.level.PlayerX;
							game.level.PlayerCarry[0].Y = game.level.PlayerY;
							game.level.itms.push_back(game.level.PlayerCarry[0]);
							game.level.CarryNum--;
						}
						else{
							game.level.CarryNum--;
							Item temp = game.level.PlayerCarry[game.level.CarryNum];
							game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
							game.level.PlayerCarry[pointer] = temp;
							game.level.working[game.level.PlayerX][game.level.PlayerY] = -3;
							game.level.PlayerCarry[game.level.CarryNum].X = game.level.PlayerX;
							game.level.PlayerCarry[game.level.CarryNum].Y = game.level.PlayerY;
							game.level.itms.push_back(game.level.PlayerCarry[game.level.CarryNum]);
							if(pointer >= game.level.CarryNum){
								pointer--;
							}
						}
					}
					else if(c == 'x' && game.level.CarryNum > 0){
						if(game.level.CarryNum == 1){
							game.level.CarryNum--;
						}
						else{
							game.level.CarryNum--;
							Item temp = game.level.PlayerCarry[game.level.CarryNum];
							game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
							game.level.PlayerCarry[pointer] = temp;
							if(pointer >= game.level.CarryNum){
								pointer--;
							}
						}
					}
					else if(c == 'w' && game.level.CarryNum > 0){
						if(game.level.PlayerCarry[pointer].type.compare("WEAPON") == 0){
							if(game.level.weaponEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.weapon;
								game.level.PlayerS -= game.level.weapon.speed;
								game.level.weapon = temp;
								game.level.PlayerS += game.level.weapon.speed;
							}
							else{
								game.level.weapon = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.weapon.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.weaponEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("OFFHAND") == 0){
							if(game.level.offhandEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.offhand;
								game.level.PlayerS -= game.level.offhand.speed;
								game.level.offhand = temp;
								game.level.PlayerS += game.level.offhand.speed;
							}
							else{
								game.level.offhand = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.offhand.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.offhandEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("RANGED") == 0){
							if(game.level.rangedEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.ranged;
								game.level.PlayerS -= game.level.ranged.speed;
								game.level.ranged = temp;
								game.level.PlayerS += game.level.ranged.speed;
							}
							else{
								game.level.ranged = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.ranged.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.rangedEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("ARMOR") == 0){
							if(game.level.armorEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.armor;
								game.level.PlayerS -= game.level.armor.speed;
								game.level.armor= temp;
								game.level.PlayerS += game.level.armor.speed;
							}
							else{
								game.level.armor = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.armor.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.armorEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("HELMET") == 0){
							if(game.level.helmetEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.helmet;
								game.level.PlayerS -= game.level.helmet.speed;
								game.level.helmet = temp;
								game.level.PlayerS += game.level.helmet.speed;
							}
							else{
								game.level.helmet = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.helmet.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.helmetEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("CLOAK") == 0){
							if(game.level.cloakEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.cloak;
								game.level.PlayerS -= game.level.cloak.speed;
								game.level.cloak = temp;
								game.level.PlayerS += game.level.cloak.speed;
							}
							else{
								game.level.cloak = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.cloak.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.cloakEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("GLOVES") == 0){
							if(game.level.glovesEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.gloves;
								game.level.PlayerS -= game.level.gloves.speed;
								game.level.gloves = temp;
								game.level.PlayerS += game.level.gloves.speed;
							}
							else{
								game.level.gloves = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.gloves.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
							game.level.glovesEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("BOOTS") == 0){
							if(game.level.bootsEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.boots;
								game.level.PlayerS -= game.level.boots.speed;
								game.level.boots = temp;
								game.level.PlayerS += game.level.boots.speed;
							}
							else{
								game.level.boots = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.boots.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.bootsEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("RING") == 0){
							if(game.level.ringAEqu && game.level.ringBEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.ringA;
								game.level.PlayerS -= game.level.ringA.speed;
								game.level.ringA = temp;
								game.level.PlayerS += game.level.ringA.speed;
							}
							else{
								if(game.level.ringAEqu){
									game.level.ringB = game.level.PlayerCarry[pointer];
									game.level.PlayerS += game.level.ringB.speed;
									if(game.level.CarryNum == 1){
										game.level.CarryNum--;
									}
									else{
										game.level.CarryNum--;
										Item temp = game.level.PlayerCarry[game.level.CarryNum];
										game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
										game.level.PlayerCarry[pointer] = temp;
										if(pointer >= game.level.CarryNum){
											pointer--;
										}
									}
									game.level.ringBEqu = true;
								}
								else{
									game.level.ringA = game.level.PlayerCarry[pointer];
									game.level.PlayerS += game.level.ringA.speed;
									if(game.level.CarryNum == 1){
										game.level.CarryNum--;
									}
									else{
										game.level.CarryNum--;
										Item temp = game.level.PlayerCarry[game.level.CarryNum];
										game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
										game.level.PlayerCarry[pointer] = temp;
										if(pointer >= game.level.CarryNum){
											pointer--;
										}
									}
									game.level.ringAEqu = true;
								}
							}
						} else if(game.level.PlayerCarry[pointer].type.compare("AMULET") == 0){
							if(game.level.amuletEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.amulet;
								game.level.PlayerS -= game.level.amulet.speed;
								game.level.amulet = temp;
								game.level.PlayerS += game.level.amulet.speed;
							}
							else{
								game.level.amulet = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.amulet.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.amuletEqu = true;
						} else if(game.level.PlayerCarry[pointer].type.compare("LIGHT") == 0){
							if(game.level.lightEqu){
								Item temp = game.level.PlayerCarry[pointer];
								game.level.PlayerCarry[pointer] = game.level.light;
								game.level.PlayerS -= game.level.light.speed;
								game.level.light = temp;
								game.level.PlayerS += game.level.light.speed;
							}
							else{
								game.level.light = game.level.PlayerCarry[pointer];
								game.level.PlayerS += game.level.light.speed;
								if(game.level.CarryNum == 1){
									game.level.CarryNum--;
								}
								else{
									game.level.CarryNum--;
									Item temp = game.level.PlayerCarry[game.level.CarryNum];
									game.level.PlayerCarry[game.level.CarryNum] = game.level.PlayerCarry[pointer];
									game.level.PlayerCarry[pointer] = temp;
									if(pointer >= game.level.CarryNum){
										pointer--;
									}
								}
							}
								game.level.lightEqu = true;
						} 
					}
					else if(c == 'I'){
						clear();
						printw("Description: ");
						printw(game.level.PlayerCarry[pointer].description.c_str());
						while(c != 27){
							c = getch();
						}
						clear();
					}
					printw("Carry Items:\n");
					for(int i = 0; i < game.level.CarryNum; ++i){
						if(i == pointer){
							printw("- ");
						}
						printw(game.level.PlayerCarry[i].name.c_str());
						printw("\n");
					}
					c = getch();
				}
			} else if(c == 'e'){
				int pointer = 0;
				while(c != 27){
					clear();
					if(c == KEY_DOWN && pointer < 11){
						pointer++;
					}
					else if(c == KEY_UP && pointer > 0){
						pointer--;
					}
					else if(c == 't' && game.level.CarryNum < 10){
						if(pointer == 0 && game.level.weaponEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.weapon;
							game.level.PlayerS -= game.level.weapon.speed;
							game.level.CarryNum++;
							game.level.weaponEqu = false;
						} else if(pointer == 1 && game.level.offhandEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.offhand;
							game.level.PlayerS -= game.level.offhand.speed;
							game.level.CarryNum++;
							game.level.offhandEqu = false;
						} else if(pointer == 2 && game.level.rangedEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.ranged;
							game.level.PlayerS -= game.level.ranged.speed;
							game.level.CarryNum++;
							game.level.rangedEqu = false;
						} else if(pointer == 3 && game.level.armorEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.armor;
							game.level.PlayerS -= game.level.armor.speed;
							game.level.CarryNum++;
							game.level.armorEqu = false;
						} else if(pointer == 4 && game.level.helmetEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.helmet;
							game.level.PlayerS -= game.level.helmet.speed;
							game.level.CarryNum++;
							game.level.helmetEqu = false;
						} else if(pointer == 5 && game.level.cloakEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.cloak;
							game.level.PlayerS -= game.level.cloak.speed;
							game.level.CarryNum++;
							game.level.cloakEqu = false;
						} else if(pointer == 6 && game.level.glovesEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.gloves;
							game.level.PlayerS -= game.level.gloves.speed;
							game.level.CarryNum++;
							game.level.glovesEqu = false;
						} else if(pointer == 7 && game.level.bootsEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.boots;
							game.level.PlayerS -= game.level.boots.speed;
							game.level.CarryNum++;
							game.level.bootsEqu = false;
						} else if(pointer == 8 && game.level.amuletEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.amulet;
							game.level.PlayerS -= game.level.amulet.speed;
							game.level.CarryNum++;
							game.level.amuletEqu = false;
						} else if(pointer == 9 && game.level.lightEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.light;
							game.level.PlayerS -= game.level.light.speed;
							game.level.CarryNum++;
							game.level.lightEqu = false;
						} else if(pointer == 10 && game.level.ringAEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.ringA;
							game.level.PlayerS -= game.level.ringA.speed;
							game.level.CarryNum++;
							game.level.ringAEqu = false;
						} else if(pointer == 11 && game.level.ringBEqu){
							game.level.PlayerCarry[game.level.CarryNum] = game.level.ringB;
							game.level.PlayerS -= game.level.ringB.speed;
							game.level.CarryNum++;
							game.level.ringBEqu = false;
						}
					}
					printw("Equipment: \n");
					for(int i = 0; i < 12; ++i){
						if(i==pointer){
							printw("- ");
						}
						if(i==0){
							if(game.level.weaponEqu){
								printw("WEAPON: ");
								printw(game.level.weapon.name.c_str());
								printw("\n");
							}
							else{
								printw("WEAPON: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==1){
							if(game.level.offhandEqu){
								printw("OFFHAND: ");
								printw(game.level.offhand.name.c_str());
								printw("\n");
							}
							else{
								printw("OFFHAND: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==2){
							if(game.level.rangedEqu){
								printw("RANGED: ");
								printw(game.level.ranged.name.c_str());
								printw("\n");
							}
							else{
								printw("RANGED: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==3){
							if(game.level.armorEqu){
								printw("ARMOR: ");
								printw(game.level.armor.name.c_str());
								printw("\n");
							}
							else{
								printw("ARMOR: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==4){
							if(game.level.helmetEqu){
								printw("HELMET: ");
								printw(game.level.helmet.name.c_str());
								printw("\n");
							}
							else{
								printw("HELMET: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==5){
							if(game.level.cloakEqu){
								printw("CLOAK: ");
								printw(game.level.cloak.name.c_str());
								printw("\n");
							}
							else{
								printw("CLOAK: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==6){
							if(game.level.glovesEqu){
								printw("GLOVES: ");
								printw(game.level.gloves.name.c_str());
								printw("\n");
							}
							else{
								printw("GLOVES: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==7){
							if(game.level.bootsEqu){
								printw("BOOTS: ");
								printw(game.level.boots.name.c_str());
								printw("\n");
							}
							else{
								printw("BOOTS: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==8){
							if(game.level.amuletEqu){
								printw("AMULET: ");
								printw(game.level.amulet.name.c_str());
								printw("\n");
							}
							else{
								printw("AMULET: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==9){
							if(game.level.lightEqu){
								printw("LIGHT: ");
								printw(game.level.light.name.c_str());
								printw("\n");
							}
							else{
								printw("LIGHT: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==10){
							if(game.level.ringAEqu){
								printw("RING A: ");
								printw(game.level.ringA.name.c_str());
								printw("\n");
							}
							else{
								printw("RING A: ");
								printw("NONE");
								printw("\n");
							}
						} else if(i==11){
							if(game.level.ringBEqu){
								printw("RING B: ");
								printw(game.level.ringB.name.c_str());
								printw("\n");
							}
							else{
								printw("RING B: ");
								printw("NONE");
								printw("\n");
							}
						}
					}
					c = getch();
				}
			} else {
				game.movePlayer(c);
			}
		} else {
			game.moveMon(min);
		}
	}

	endwin();
	free(home);
	free(path);
	return 0;
}
