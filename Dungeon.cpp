/*
 * Dungeon.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#include <iostream>
#include <ncurses.h>
#include "Dungeon.h"
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

int dungeon::checkClearence(int locX, int locY, int dimX, int dimY) {
	int i, j, n;
	for (i = 0; i < (2 * dimY) + 1; ++i) {
		for (j = 0; j < (2 * dimX) + 1; ++j) {
			if (dun[(locX - dimX) + j][(locY - dimY) + i] != 1) {
				return 0;
			}
			for (n = locY - dimY - 1; n < locY + dimY + 1; ++n) {
				if (dun[locX + dimX + 2][n] != 1) {
					return 0;
				}
				if (dun[locX - dimX - 2][n] != 1) {
					return 0;
				}
			}
			for (n = locX - dimX - 1; n < locX + dimX + 1; ++n) {
				if (dun[n][locY + dimY + 2] != 1) {
					return 0;
				}
				if (dun[n][locY - dimY - 2] != 1) {
					return 0;
				}
			}
		}
	}
	return 1;
}

dungeon::dungeon() {
	PlayerS = 0;
	done = false;
	PlayerHP = 100;
	CarryNum = 0;
	toggleFog = false;
	int i, j, num;
	for (i = 0; i < 21; i++) {
		for (j = 0; j < 80; j++) {
			dun[j][i] = 1;
			found[j][i] = false;
		}
	}

	for (i = 0; i < 80; ++i) {
		dun[i][0] = 0;
		dun[i][20] = 0;
	}
	for (i = 0; i < 21; ++i) {
		dun[0][i] = 0;
		dun[79][i] = 0;
	}

	num = rand() % (8 + 1 - 5) + 5;
	for (i = 0; i < 9; ++i) {
		for (j = 0; j < 4; ++j) {
			rooms[j][i] = -1;
		}
	}

	int locX, locY, dimX, dimY;
	int created = 0;
	int roomsX[num];
	int roomsY[num];
	int roomDimX[num];
	int roomDimY[num];
	while (created < num) {
		locX = rand() % (69 + 1 - 10) + 10;
		locY = rand() % (15 + 1 - 5) + 5;
		dimX = rand() % (7 + 1 - 1) + 1;
		dimY = rand() % (4 + 1 - 1) + 1;
		if (checkClearence(locX, locY, dimX, dimY) == 1) {
			for (i = 0; i < (2 * dimY) + 1; ++i) {
				for (j = 0; j < (2 * dimX) + 1; ++j) {
					dun[(locX - dimX) + j][(locY - dimY) + i] = 2;
				}
			}

			roomsX[created] = locX;
			roomsY[created] = locY;
			roomDimX[created] = dimX;
			roomDimY[created] = dimY;
			rooms[0][created] = locX;
			rooms[1][created] = locY;
			rooms[2][created] = dimX;
			rooms[3][created] = dimY;
			created++;
		}
	}
	for (i = 0; i < num - 1; ++i) {
		int loc1[4];
		int loc2[4];
		int val1 = (0 + i);
		int val2 = (1 + i);
		loc1[0] = roomsX[val1];
		loc1[1] = roomsY[val1];
		loc1[2] = roomDimX[val1];
		loc1[3] = roomDimY[val1];
		loc2[0] = roomsX[val2];
		loc2[1] = roomsY[val2];
		loc2[2] = roomDimX[val2];
		loc2[3] = roomDimY[val2];

		int loc[2];
		loc[0] = loc1[0];
		loc[1] = loc2[1];
		int cur[2];
		cur[0] = loc1[0];
		cur[1] = loc1[1];
		while (cur[1] != loc[1]) {
			if (dun[cur[0]][cur[1]] != 1) {
				if (cur[1] < loc[1]) {
					cur[1] = cur[1] + 1;
				} else {
					cur[1] = cur[1] - 1;
				}
			} else {
				dun[cur[0]][cur[1]] = 3;
				if (cur[1] < loc[1]) {
					cur[1] = cur[1] + 1;
				} else {
					cur[1] = cur[1] - 1;
				}
			}
		}
		while (cur[0] != loc2[0]) {
			if (dun[cur[0]][cur[1]] != 1) {
				if (cur[0] < loc2[0]) {
					cur[0] = cur[0] + 1;
				} else {
					cur[0] = cur[0] - 1;
				}
			} else {
				dun[cur[0]][cur[1]] = 3;
				if (cur[0] < loc2[0]) {
					cur[0] = cur[0] + 1;
				} else {
					cur[0] = cur[0] - 1;
				}
			}
		}
	}
	for (j = 0; j < 21; ++j) {
		for (i = 0; i < 80; ++i) {
			if (dun[i][j] == 0) {
				hardness[i][j] = 255;
			} else if (dun[i][j] == 2 || dun[i][j] == 3) {
				hardness[i][j] = 0;
			} else if (dun[i][j] == 1) {
				hardness[i][j] = (rand() % (254 - 10)) + 10;
			}
			Tunneling[i][j] = -1;
			nonTunneling[i][j] = -1;
			working[i][j] = dun[i][j];
		}
	}

	dun[rooms[0][0]][rooms[1][0]] = 4;
	PlayerX = rooms[0][0];
	PlayerY = rooms[1][0];
	upX = rooms[0][0] - 1;
	upY = rooms[1][0];
	downX = rooms[0][1] - 1;
	downY = rooms[1][1];
	dun[downX][downY] = -1;
	dun[upX][upY] = -2;
	working[downX][downY] = -1;
	working[upX][upY] = -2;

	weaponEqu = false;
	offhandEqu = false;
	rangedEqu = false;
	armorEqu = false;
	helmetEqu = false;
	cloakEqu = false;
	glovesEqu = false;
	bootsEqu = false;
	amuletEqu = false;
	lightEqu = false;
	ringAEqu = false;
	ringBEqu = false;
}

dungeon::~dungeon(){

}

void dungeon::drawdun() {
	if (toggleFog) {
		int i, j;
		for (i = 0; i < 21; i++) {
			for (j = 0; j < 80; j++) {
				if (dun[j][i] == -2) {
					printw("<");
				} else if (dun[j][i] == -1) {
					printw(">");
				} else if (dun[j][i] == 0) {
					if (i == 0 || i == 20) {
						printw("-");
					} else if (j == 0 || j == 79) {
						printw("|");
					}
				} else if (dun[j][i] == 1) {
					printw(" ");
				} else if (dun[j][i] == 2) {
					printw(".");
				} else if (dun[j][i] == 3) {
					printw("#");
				} else if (dun[j][i] == 4) {
					printw("@");
				} else if (dun[j][i] == 5) {
					bool lost = true;
					for(unsigned int n = 0; n < mons.size(); ++n){
						if(mons.at(n).X == j && mons.at(n).Y == i){
							init_pair((short) mons.at(n).color, (short) mons.at(n).color, 0);
							attron(COLOR_PAIR(mons.at(n).color));
							printw("%c", mons.at(n).symb);
							attroff(COLOR_PAIR(mons.at(n).color));
							lost = false;
							
						}
					}
					if(lost){
						printw("?");
					}
				}else if (dun[j][i] == 21) {
					printw("*");
				} else {
					bool lost = true;
					for(unsigned int n = 0; n < itms.size(); ++n){
						if(itms.at(n).X == j && itms.at(n).Y == i){
							init_pair((short) itms.at(n).color, (short) itms.at(n).color, 0);
							//attron(COLOR_PAIR(itms.at(n).color));
							if(itms.at(n).type.compare("WEAPON") == 0){
								printw("|");
							} else if(itms.at(n).type.compare("OFFHAND") == 0){
								printw(")");
							} else if(itms.at(n).type.compare("RANGED") == 0){
								printw("}");
							} else if(itms.at(n).type.compare("ARMOR") == 0){
								printw("[");
							} else if(itms.at(n).type.compare("HELMET") == 0){
								printw("]");
							} else if(itms.at(n).type.compare("CLOAK") == 0){
								printw("(");
							} else if(itms.at(n).type.compare("GLOVES") == 0){
								printw("{");
							} else if(itms.at(n).type.compare("BOOTS") == 0){
								printw("\\");
							} else if(itms.at(n).type.compare("RING") == 0){
								printw("=");
							} else if(itms.at(n).type.compare("AMULET") == 0){
								printw("\"");
							} else if(itms.at(n).type.compare("LIGHT") == 0){
								printw("_");
							} else if(itms.at(n).type.compare("SCROLL") == 0){
								printw("~");
							} else if(itms.at(n).type.compare("BOOK") == 0){
								printw("?");
							} else if(itms.at(n).type.compare("FLASK") == 0){
								printw("!");
							} else if(itms.at(n).type.compare("GOLD") == 0){
								printw("$");
							} else if(itms.at(n).type.compare("AMMUNITION") == 0){
								printw("/");
							} else if(itms.at(n).type.compare("FOOD") == 0){
								printw(",");
							} else if(itms.at(n).type.compare("WAND") == 0){
								printw("-");
							} else if(itms.at(n).type.compare("CONTAINER") == 0){
								printw("%");
							} else{
								printw("&");
							}
							
							//attroff(COLOR_PAIR(itms.at(n).color));
							lost = false;
							
						}
					}
					if(lost){
						printw("?");
					}
				}
			}
			printw("\n");
		}
		return;
	}
	int i, j;
	for (i = 0; i < 21; i++) {
		for (j = 0; j < 80; j++) {
			if (working[j][i] == 0) {
				if (i == 0 || i == 20) {
					printw("-");
				} else if (j == 0 || j == 79) {
					printw("|");
				}
			} else if (dun[j][i] == 21) {
				printw("*");
			}else if (dun[j][i] == 4) {
				printw("@");
			} else if (found[j][i]) {
				if (j >= PlayerX - 2 && j <= PlayerX + 2 && i >= PlayerY - 2
						&& i <= PlayerY + 2) {
					if (dun[j][i] == 5) {
						for(unsigned int n = 0; n < mons.size(); ++n){
							if(mons.at(n).X == j && mons.at(n).Y == i){
								init_pair((short) mons.at(n).color, (short) mons.at(n).color, 0);
								attron(COLOR_PAIR(mons.at(n).color));
								printw("%c", mons.at(n).symb);
								attroff(COLOR_PAIR(mons.at(n).color));
							}
						}
					} else if (working[j][i] == -2) {
						printw("<");
					} else if (working[j][i] == -1) {
						printw(">");
					} else if (working[j][i] == 2) {
						printw(".");
					} else if (dun[j][i] == 3) {
						printw("#");
					} else {
						bool lost = true;
						for(unsigned int n = 0; n < itms.size(); ++n){
							if(itms.at(n).X == j && itms.at(n).Y == i){
								init_pair((short) itms.at(n).color, (short) itms.at(n).color, 0);
								//attron(COLOR_PAIR(itms.at(n).color));
								if(itms.at(n).type.compare("WEAPON") == 0){
								printw("|");
							} else if(itms.at(n).type.compare("OFFHAND") == 0){
								printw(")");
							} else if(itms.at(n).type.compare("RANGED") == 0){
								printw("}");
							} else if(itms.at(n).type.compare("ARMOR") == 0){
								printw("[");
							} else if(itms.at(n).type.compare("HELMET") == 0){
								printw("]");
							} else if(itms.at(n).type.compare("CLOAK") == 0){
								printw("(");
							} else if(itms.at(n).type.compare("GLOVES") == 0){
								printw("{");
							} else if(itms.at(n).type.compare("BOOTS") == 0){
								printw("\\");
							} else if(itms.at(n).type.compare("RING") == 0){
								printw("=");
							} else if(itms.at(n).type.compare("AMULET") == 0){
								printw("\"");
							} else if(itms.at(n).type.compare("LIGHT") == 0){
								printw("_");
							} else if(itms.at(n).type.compare("SCROLL") == 0){
								printw("~");
							} else if(itms.at(n).type.compare("BOOK") == 0){
								printw("?");
							} else if(itms.at(n).type.compare("FLASK") == 0){
								printw("!");
							} else if(itms.at(n).type.compare("GOLD") == 0){
								printw("$");
							} else if(itms.at(n).type.compare("AMMUNITION") == 0){
								printw("/");
							} else if(itms.at(n).type.compare("FOOD") == 0){
								printw(",");
							} else if(itms.at(n).type.compare("WAND") == 0){
								printw("-");
							} else if(itms.at(n).type.compare("CONTAINER") == 0){
								printw("%");
							} else{
								printw("&");
							}
								//attroff(COLOR_PAIR(itms.at(n).color));
								lost = false;
							
							}
						}
						if(lost){
							printw("?");
						}
					}

				} else if (working[j][i] == -2) {
					printw("<");
				} else if (working[j][i] == -1) {
					printw(">");
				} else if (working[j][i] == 2) {
					printw(".");
				} else if (working[j][i] == 3) {
					printw("#");
				} else if(working[j][i] == 1){
					printw(" ");
				} else {
					bool lost = true;
						for(unsigned int n = 0; n < itms.size(); ++n){
							if(itms.at(n).X == j && itms.at(n).Y == i){
								init_pair((short) itms.at(n).color, (short) itms.at(n).color, 0);
								//attron(COLOR_PAIR(itms.at(n).color));
								if(itms.at(n).type.compare("WEAPON") == 0){
								printw("|");
							} else if(itms.at(n).type.compare("OFFHAND") == 0){
								printw(")");
							} else if(itms.at(n).type.compare("RANGED") == 0){
								printw("}");
							} else if(itms.at(n).type.compare("ARMOR") == 0){
								printw("[");
							} else if(itms.at(n).type.compare("HELMET") == 0){
								printw("]");
							} else if(itms.at(n).type.compare("CLOAK") == 0){
								printw("(");
							} else if(itms.at(n).type.compare("GLOVES") == 0){
								printw("{");
							} else if(itms.at(n).type.compare("BOOTS") == 0){
								printw("\\");
							} else if(itms.at(n).type.compare("RING") == 0){
								printw("=");
							} else if(itms.at(n).type.compare("AMULET") == 0){
								printw("\"");
							} else if(itms.at(n).type.compare("LIGHT") == 0){
								printw("_");
							} else if(itms.at(n).type.compare("SCROLL") == 0){
								printw("~");
							} else if(itms.at(n).type.compare("BOOK") == 0){
								printw("?");
							} else if(itms.at(n).type.compare("FLASK") == 0){
								printw("!");
							} else if(itms.at(n).type.compare("GOLD") == 0){
								printw("$");
							} else if(itms.at(n).type.compare("AMMUNITION") == 0){
								printw("/");
							} else if(itms.at(n).type.compare("FOOD") == 0){
								printw(",");
							} else if(itms.at(n).type.compare("WAND") == 0){
								printw("-");
							} else if(itms.at(n).type.compare("CONTAINER") == 0){
								printw("%");
							} else{
								printw("&");
							}
								//attroff(COLOR_PAIR(itms.at(n).color));
								lost = false;
							
							}
						}
						if(lost){
							printw("?");
						}
				}
			} else {
				printw(" ");
			}
		}
		printw("\n");
	}
}

void dungeon::calculateNonTunneling() {
	int i, j;
	int Q[80][21];
	int Qsize = 0;

	for (i = 0; i < 21; ++i) {
		for (j = 0; j < 80; ++j) {
			if (hardness[j][i] == 0) {
				nonTunneling[j][i] = -1;
				if (j == PlayerX && i == PlayerY) {
					nonTunneling[j][i] = 0;
				}
				Q[j][i] = 1;
				Qsize++;
			} else {
				Q[j][i] = -1;
				nonTunneling[j][i] = 1000000;
			}
		}
	}

	while (Qsize != 0) {
		int minX = 0;
		int minY = 0;
		int min = nonTunneling[0][0];
		for (i = 0; i < 21; ++i) {
			for (j = 0; j < 80; ++j) {
				if (nonTunneling[j][i] < min && nonTunneling[j][i] != -1
						&& Q[j][i] == 1) {
					min = nonTunneling[j][i];
					minX = j;
					minY = i;
				}
			}
		}
		Q[minX][minY] = 0;
		Qsize--;
		int move[8][2];
		move[0][0] = -1;
		move[0][1] = -1;
		move[1][0] = 0;
		move[1][1] = -1;
		move[2][0] = 1;
		move[2][1] = -1;
		move[3][0] = -1;
		move[3][1] = 0;
		move[4][0] = 1;
		move[4][1] = 0;
		move[5][0] = -1;
		move[5][1] = 1;
		move[6][0] = 0;
		move[6][1] = 1;
		move[7][0] = 1;
		move[7][1] = 1;
		for (i = 0; i < 8; ++i) {
			if (minX + move[i][0] >= 0 && minX + move[i][0] < 80
					&& minY + move[i][1] >= 0 && minY + move[i][1] < 21) {
				int alt = nonTunneling[minX][minY] + 1;
				if (Q[minX + move[i][0]][minY + move[i][1]] == 1) {
					if (alt < nonTunneling[minX + move[i][0]][minY + move[i][1]]
							|| nonTunneling[minX + move[i][0]][minY + move[i][1]]
									== -1) {
						nonTunneling[minX + move[i][0]][minY + move[i][1]] =
								alt;
					}
				}
			}
		}

	}
}

void dungeon::calculateTunneling() {
	int i, j;
	int Q[80][21];
	int Qsize = 0;

	for (i = 0; i < 21; ++i) {
		for (j = 0; j < 80; ++j) {
			if (hardness[j][i] < 255) {
				Tunneling[j][i] = -1;
				if (j == PlayerX && i == PlayerY) {
					Tunneling[j][i] = 0;
				}
				Q[j][i] = 1;
				Qsize++;
			} else {
				Q[j][i] = -1;
				Tunneling[j][i] = 1000000;
			}
		}
	}

	while (Qsize != 0) {
		int minX = 0;
		int minY = 0;
		int min = Tunneling[0][0];
		for (i = 0; i < 21; ++i) {
			for (j = 0; j < 80; ++j) {
				if (Tunneling[j][i] < min && Tunneling[j][i] != -1
						&& Q[j][i] == 1) {
					min = Tunneling[j][i];
					minX = j;
					minY = i;
				}
			}
		}
		Q[minX][minY] = 0;
		Qsize--;

		int move[8][2];
		move[0][0] = -1;
		move[0][1] = -1;
		move[1][0] = 0;
		move[1][1] = -1;
		move[2][0] = 1;
		move[2][1] = -1;
		move[3][0] = -1;
		move[3][1] = 0;
		move[4][0] = 1;
		move[4][1] = 0;
		move[5][0] = -1;
		move[5][1] = 1;
		move[6][0] = 0;
		move[6][1] = 1;
		move[7][0] = 1;
		move[7][1] = 1;
		for (i = 0; i < 8; ++i) {
			if (minX + move[i][0] >= 0 && minX + move[i][0] < 80
					&& minY + move[i][1] >= 0 && minY + move[i][1] < 21
					&& Q[minX + move[i][0]][minY + move[i][1]] == 1) {
				int alt;
				if (hardness[minX + move[i][0]][minY + move[i][1]] == 0) {
					alt = Tunneling[minX][minY] + 1;
				} else if (hardness[minX + move[i][0]][minY + move[i][1]]
						< 255) {
					alt = Tunneling[minX][minY]
							+ (((hardness[minX][minY]) / 85) + 1);
				} else {
					continue;
				}

				if (alt < Tunneling[minX + move[i][0]][minY + move[i][1]]
						|| Tunneling[minX + move[i][0]][minY + move[i][1]]
								== -1) {
					Tunneling[minX + move[i][0]][minY + move[i][1]] = alt;
				}

			}
		}

	}
}

int dungeon::PlayerAttack(){
	if(weaponEqu){
		int dam = roll("0+1d4");
		if(weaponEqu){
			dam+=roll(weapon.damage);
		}
		if(offhandEqu){
			dam+=roll(offhand.damage);
		}
		if(rangedEqu){
			dam+=roll(ranged.damage);
		}
		if(armorEqu){
			dam+=roll(armor.damage);
		}
		if(helmetEqu){
			dam+=roll(helmet.damage);
		}
		if(cloakEqu){
			dam+=roll(cloak.damage);
		}
		if(glovesEqu){
			dam+=roll(gloves.damage);
		}
		if(bootsEqu){
			dam+=roll(boots.damage);
		}
		if(amuletEqu){
			dam+=roll(amulet.damage);
		}
		if(lightEqu){
			dam+=roll(light.damage);
		}
		if(ringAEqu){
			dam+=roll(ringA.damage);
		}
		if(ringBEqu){
			dam+=roll(ringB.damage);
		}
		return dam;
	}
	else{
		return roll("0+1d4");
	}
}

int dungeon::roll(std::string die){
	int indP = 0;
	int count = 0;
	while(die.at(indP) != '+'){
		indP++;
		if(count == 100){
			return 0;
		}
		count++;
	}
	int indD = indP + 1;
	count = 0;
	while(die.at(indD) != 'd'){
		indD++;
		if(count == 100){
			return 0;
		}
		count++;
	}
	int min = std::stoi(die.substr(0,indP));
	int dice = std::stoi(die.substr(indP+1,indD));
	int sides = std::stoi(die.substr(indD+1));
	int total = 0;
	for(int i = 0; i < dice; i++){
		total += rand()%sides+1;
	}
	return min + total;
}
