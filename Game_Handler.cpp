/*
 * Game_Handler.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#include "Game_Handler.h"
#include <iostream>
#include <ncurses.h>
#include <limits.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include "Save-Load.h"
#include "Item.h"

Game_Handler::Game_Handler(bool ld, int monNum, std::vector<parMon> monList, std::vector<parItm> itmLst) {
	mon = monList;
	itm = itmLst;
	numberOfMon = monNum;
	moveQ = (int *) malloc(sizeof(int) * (numberOfMon + 1));
	monLst = (monster *) malloc(sizeof(monster) * numberOfMon);
	levelNum = 0;
	if (ld) {
		level = load();
	} else {
		dungeon dun;
		level = dun;
	}
	levelMons = (monster*) malloc(sizeof(monster) * numberOfMon);
	PCAlive = true;
	s = "Begin Game\n";
	generateMonsters(monList, itmLst);
}

Game_Handler::~Game_Handler() {
	free(moveQ);
	free(monLst);
	free(levelMons);
}

void Game_Handler::generateMonsters(std::vector<parMon> monList, std::vector<parItm> itmLst) {
	int i;
	for (i = 0; i < numberOfMon; i++) {
		monster mon(level);
		while(true){
			int monD = rand()%monList.size();
			int rar = rand()%100;
			if(monList.at(monD).rarity > rar){
				if(monList.at(monD).Unique && monList.at(monD).found){
					continue;
				}
				mon = monList.at(monD).gen(level);
				if(monList.at(monD).Unique){
					monList.at(monD).found = true;
				}
				break;
			}
		}
		moveQ[i] = (1000 / mon.speed);
		monLst[i] = mon;
		level.mons.push_back(mon);
		level.dun[mon.X][mon.Y] = 5;
	}
	moveQ[numberOfMon] = 100;

	for(int i = 0; i < 12; ++i){
		Item thing;
		
		while(true){
			int itmD = rand()%itmLst.size();
			int rar = rand()%100;
			if(itmLst.at(itmD).rarity > rar){
				if(itmLst.at(itmD).artifact.compare("TRUE")==0 && itmLst.at(itmD).found){
					continue;
				}
				thing = itmLst.at(itmD).gen();
				if(itmLst.at(itmD).artifact.compare("TRUE")==0){
					itmLst.at(itmD).found = true;
				}
				break;
			}
		}

		while(true){
			int Xloc = rand() % (78) + 1;
			int Yloc = rand() % (19) + 1;
			if(level.dun[Xloc][Yloc] == 2 || level.dun[Xloc][Yloc] == 3){
				thing.X = Xloc;
				thing.Y = Yloc;
				level.dun[Xloc][Yloc] = -3;
				level.working[Xloc][Yloc] = -3;
				break;
			}
		}
		level.itms.push_back(thing);
	}
}

int Game_Handler::checkEnd() {
	if (level.PlayerHP == 0) {
		return 1;
	}
	if (level.done) {
		return 2;
	}
	return 0;
}

void Game_Handler::movePlayer(int c) {
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
	int mov, i;
	mov = 0;
	if (c == '7' || c == 'y') {
		s = "Move Up-Left\n";
		mov = 0;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '8' || c == 'k') {
		s = "Move Up\n";
		mov = 1;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '9' || c == 'u') {
		s = "Move Up-Right\n";
		mov = 2;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '6' || c == 'l') {
		s = "Move Right\n";
		mov = 4;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '3' || c == 'n') {
		s = "Move Down-Right\n";
		mov = 7;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '2' || c == 'j') {
		s = "Move Down\n";
		mov = 6;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '1' || c == 'b') {
		s = "Move Down-Left\n";
		mov = 5;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '4' || c == 'h') {
		s = "Move Left\n";
		mov = 3;
		if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY
				+ move[mov][1]] != 0) {
			clear();
			s = "Invalid Move: Try Again\n";
			printw(s);
			level.drawdun();
			refresh();
		}
	} else if (c == '5' || c == ' ') {
		moveQ[numberOfMon] = moveQ[numberOfMon] + 100;
		clear();
		s = "Rest for a turn\n";
		printw(s);
		level.drawdun();
		refresh();
		return;
	} else if (c == '>') {
		if (level.PlayerX == level.downX && level.PlayerY == level.downY) {
			levelNum++;
			dungeon dun;
			level = dun;
			generateMonsters(mon,itm);
			dun.calculateTunneling();
			dun.calculateNonTunneling();
			s = "New Level\n";
			return;
		}
	} else if (c == '<') {
		if (level.PlayerX == level.upX && level.PlayerY == level.upY) {
			levelNum--;
			dungeon dun;
			level = dun;
			generateMonsters(mon,itm);
			dun.calculateTunneling();
			dun.calculateNonTunneling();
			s = "New Level\n";
			return;
		}
	} else if (c == 'm') {
		int base;
		base = 0;
		int w;
		while (true) {
			clear();
			printw("monster list: \n");
			for (w = 0; w < 23 && base + w < numberOfMon; w++) {
				if (monLst[base + w].exists) {
					printw("Alive: ");
				} else {
					printw("Dead: ");
				}
				printw("%c", monLst[base + w].symb);
				int x = level.PlayerX - monLst[base + w].X;
				int y = level.PlayerY - monLst[base + w].Y;
				if (x < 0) {
					printw(", is right %d", x * -1);
				} else {
					printw(", is left %d", x);
				}
				if (y < 0) {
					printw(", and down %d", y * -1);
				} else {
					printw(", is up %d", y);
				}
				printw(": It has a speed of %d\n", monLst[base + w].speed);
			}
			refresh();
			c = getch();
			if (c == 27) {
				clear();
				s = "Back To Game\n";
				printw(s);
				level.drawdun();
				refresh();
				return;
			} else if (c == KEY_DOWN) {
				if (base + 23 < numberOfMon) {
					base++;
				}
			} else if (c == KEY_UP) {
				if (base != 0) {
					base--;
				}
			}
		}
	} else {
		clear();
		s = "Invalid Command: Try Again\n";
		printw(s);
		level.drawdun();
		refresh();
		return;
	}

	if (level.dun[level.PlayerX+ move[mov][0]][level.PlayerY+ move[mov][1]] == 5) {
			for (i = 0; i < numberOfMon; ++i) {
				if (monLst[i].X == level.PlayerX+ move[mov][0] && monLst[i].Y == level.PlayerY+ move[mov][1]) {
					int dam = level.PlayerAttack();
					if(monLst[i].HP < dam){
						monLst[i].exists = 0;
						moveQ[i] = INT_MAX;
						if(monLst[i].Boss){
							level.done = true;
						}
						level.dun[level.PlayerX+ move[mov][0]][level.PlayerY+ move[mov][1]] = level.working[level.PlayerX+ move[mov][0]][level.PlayerY+ move[mov][1]];
					} else{
						monLst[i].HP -= dam;
					}
				}
			}
		moveQ[numberOfMon] = moveQ[numberOfMon] + (1000/(10+level.PlayerS));
			return;
		}

	if (level.hardness[level.PlayerX + move[mov][0]][level.PlayerY + move[mov][1]] == 0) {
		if (level.working[level.PlayerX][level.PlayerY] == 2) {
			level.dun[level.PlayerX][level.PlayerY] = 2;
		} else if (level.working[level.PlayerX][level.PlayerY] == -1) {
			level.dun[level.PlayerX][level.PlayerY] = -1;
		} else if (level.working[level.PlayerX][level.PlayerY] == -2) {
			level.dun[level.PlayerX][level.PlayerY] = -2;
		}else if (level.working[level.PlayerX][level.PlayerY] == -3) {
			level.dun[level.PlayerX][level.PlayerY] = -3;
		} else {
			level.dun[level.PlayerX][level.PlayerY] = 3;
		}
		level.PlayerX = level.PlayerX + move[mov][0];
		level.PlayerY = level.PlayerY + move[mov][1];
		if(level.working[level.PlayerX][level.PlayerY] == -3 && level.CarryNum < 10){
			if((level.working[level.PlayerX][level.PlayerY+1] == 3 && level.working[level.PlayerX][level.PlayerY-1] == 3) || (level.working[level.PlayerX+1][level.PlayerY] == 3 && level.working[level.PlayerX-1][level.PlayerY] == 3)){
				level.working[level.PlayerX][level.PlayerY] = 3;
			}
			else{
				level.working[level.PlayerX][level.PlayerY] = 2;
			}
			for(unsigned int i = 0; i < level.itms.size(); ++i){
				if(level.itms.at(i).X == level.PlayerX && level.itms.at(i).Y == level.PlayerY){
					level.PlayerCarry[level.CarryNum] = level.itms.at(i);
					level.CarryNum++;
					level.itms.erase(level.itms.begin()+i);
				}
			}
		}
		level.dun[level.PlayerX][level.PlayerY] = 4;
		moveQ[numberOfMon] = moveQ[numberOfMon] + (1000/(10+level.PlayerS));
		level.calculateNonTunneling();
		level.calculateTunneling();
	}
}

void Game_Handler::moveMon(int min) {
	int i;
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
	int mov = monLst[min].findMove(level);
	if (level.dun[monLst[min].X+move[mov][0]][monLst[min].Y+move[mov][1]] == 4) {
		int dam = monLst[min].DamageBase;
		for(int i = 0; i < monLst[min].DamageNum; ++i){
			dam += rand()%monLst[min].DamageSides+1;
		}
		if(level.PlayerHP < dam){
			level.PlayerHP = 0;
		}
		else{
			level.PlayerHP -= dam;
		}
		moveQ[min] = moveQ[min] + (1000 / monLst[min].speed);
		return;
	}
	if (monLst[min].X + move[mov][0] < 0 || monLst[min].X + move[mov][0] > 79
			|| monLst[min].Y + move[mov][1] < 0
			|| monLst[min].Y + move[mov][1] > 20) {
		return;
	}
	if (level.hardness[monLst[min].X + move[mov][0]][monLst[min].Y + move[mov][1]] > 85) {
		if (monLst[min].Tunneling == 1 && level.hardness[monLst[min].X + move[mov][0]][monLst[min].Y + move[mov][1]] < 255) {
			level.hardness[monLst[min].X + move[mov][0]][monLst[min].Y + move[mov][1]] = level.hardness[monLst[min].X + move[mov][0]][monLst[min].Y + move[mov][1]] - 85;
			moveQ[min] = moveQ[min] + (1000 / monLst[min].speed);
			level.calculateNonTunneling();
			level.calculateTunneling();
			return;
		} else {
			moveQ[min] = moveQ[min] + (1000 / monLst[min].speed);
			return;
		}
	}
	if (monLst[min].Tunneling == 0
			&& level.hardness[monLst[min].X + move[mov][0]][monLst[min].Y
					+ move[mov][1]] != 0) {
		moveQ[min] = moveQ[min] + (1000 / monLst[min].speed);
		return;
	}
	if (level.working[monLst[min].X][monLst[min].Y] == 2) {
		level.dun[monLst[min].X][monLst[min].Y] = 2;
	} else if (level.working[monLst[min].X][monLst[min].Y] == -1) {
		level.dun[monLst[min].X][monLst[min].Y] = -1;
	} else if (level.working[monLst[min].X][monLst[min].Y] == -2) {
		level.dun[monLst[min].X][monLst[min].Y] = -2;
	}else if (level.working[monLst[min].X][monLst[min].Y] == -3) {
		level.dun[monLst[min].X][monLst[min].Y] = -3;
	} else {
		level.dun[monLst[min].X][monLst[min].Y] = 3;
	}
	level.hardness[monLst[min].X + move[mov][0]][monLst[min].Y + move[mov][1]] = 0;
	monLst[min].X = (monLst[min].X + move[mov][0]);
	monLst[min].Y = (monLst[min].Y + move[mov][1]);
	level.mons.at(min).X = monLst[min].X;
	level.mons.at(min).Y = monLst[min].Y;
	if (level.dun[monLst[min].X][monLst[min].Y] == 5) {
		for (i = 0; i < numberOfMon; ++i) {
			if (monLst[i].X == monLst[min].X && monLst[i].Y == monLst[min].Y && i != min) {
				monLst[i].X = (monLst[min].X - move[mov][0]);
				monLst[i].Y = (monLst[min].Y - move[mov][1]);
				level.mons.at(i).X = monLst[min].X;
				level.mons.at(i).Y = monLst[min].Y;
				level.calculateNonTunneling();
				level.calculateTunneling();
			}
		}
	}
	level.dun[monLst[min].X][monLst[min].Y] = 5;
	moveQ[min] = moveQ[min] + (1000 / monLst[min].speed);

}

void Game_Handler::teleport() {
	level.dun[level.PlayerX][level.PlayerY] = 21;
	int telX = level.PlayerX;
	int telY = level.PlayerY;
	int save = 4;
	clear();
	printw("Selecting Teleport\n");
	level.drawdun();
	refresh();
	int c = getch();
	while (c != 'g') {
		switch (c) {
		case 'k':
			level.dun[telX][telY] = save;
			telY--;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'j':
			level.dun[telX][telY] = save;
			telY++;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'h':
			level.dun[telX][telY] = save;
			telX--;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'l':
			level.dun[telX][telY] = save;
			telX++;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'u':
			level.dun[telX][telY] = save;
			telX++;
			telY--;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'y':
			level.dun[telX][telY] = save;
			telX--;
			telY--;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'n':
			level.dun[telX][telY] = save;
			telX++;
			telY++;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'b':
			level.dun[telX][telY] = save;
			telX--;
			telY++;
			save = level.dun[telX][telY];
			level.dun[telX][telY] = 21;
			break;
		case 'r':
			level.dun[level.PlayerX][level.PlayerY] = level.working[level.PlayerX][level.PlayerY];
			telX = (rand() % 75)+2;
			telY= (rand() % 17)+2;
			level.dun[telX][telY] = 21;
			level.dun[telX][telY] = 4;
			level.PlayerX = telX;
			level.PlayerY = telY;
			return;
		}
		clear();
		printw("Selecting Teleport\n");
		level.drawdun();
		refresh();
		c = getch();
	}
	if(level.hardness[telX][telY] != 255){
		level.dun[level.PlayerX][level.PlayerY] = level.working[level.PlayerX][level.PlayerY];
		level.dun[telX][telY] = 4;
		level.PlayerX = telX;
		level.PlayerY = telY;
		int i;
		for(i = 0; i < numberOfMon; ++i){
			if(monLst[i].X == level.PlayerX && monLst[i].Y == level.PlayerY){
				monLst[i].exists = 0;
			}
		}
	}
	else{
		level.dun[telX][telY] = 0;
	}
}
