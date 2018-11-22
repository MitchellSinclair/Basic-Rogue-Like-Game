/*
 * monster.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#include "monster.h"
#include "Dungeon.h"
#include <ncurses.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>

monster::monster(dungeon dun) {
	int boolean = rand() % 2;
	Intelligence = boolean;
	boolean = rand() % 2;
	Erratic = boolean;
	boolean = rand() % 2;
	Telepathy = boolean;
	boolean = rand() % 2;
	Tunneling = boolean;
	boolean = 0;
	while (boolean != 1) {
		int Xloc = rand() % (78) + 1;
		int Yloc = rand() % (19) + 1;
		if (dun.hardness[Xloc][Yloc] == 0) {
			if (Xloc < dun.PlayerX - dun.rooms[2][0]
					|| Xloc > dun.PlayerX + dun.rooms[2][0]) {
				if (Yloc < dun.PlayerY - dun.rooms[3][0]
						|| Yloc > dun.PlayerY + dun.rooms[3][0]) {
					boolean = 1;
					X = Xloc;
					Y = Yloc;
				}
			}
		}
	}
	num = (Erratic * 8) + (Tunneling * 4) + (Telepathy * 2) + (Intelligence)
			+ 5;
	speed = rand() % (16) + 5;
	exists = 1;
}

monster::monster() {

}

monster::~monster(){

}

int monster::canSee(dungeon dun) {
	int n, j;
	n = 0;
	j = 0;
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

	for (n = 0; n < 9; ++n) {
		if (X < dun.rooms[n][0] + dun.rooms[n][2]
				&& Y < dun.rooms[n][1] + dun.rooms[n][3]) {
			if (X > dun.rooms[n][0] - dun.rooms[n][2]
					&& Y > dun.rooms[n][1] - dun.rooms[n][3]) {
				if (dun.PlayerX < dun.rooms[n][0] + dun.rooms[n][2]
						&& dun.PlayerY < dun.rooms[n][1] + dun.rooms[n][3]) {
					if (dun.PlayerX > dun.rooms[n][0] - dun.rooms[n][2]) {
						return dun.PlayerY > dun.rooms[n][1] - dun.rooms[n][3];
					}
				}
			}
		}
	}

	for (n = 0; n < 8; ++n) {
		for (j = 0; j < 5; ++j) {
			if (dun.PlayerX == X + move[n][0] * j) {
				if (dun.PlayerY == Y + move[n][1] * j) {
					return 1;
				}
			}
		}
	}
	return 0;
}

int monster::findMove(dungeon dun) {
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

	if (Erratic == 1) {
		if (Tunneling == 1) {
			int boolean = rand() % 2;
			if (boolean == 1) {
				return rand() % 8;
			}
		} else {
			int boolean = rand() % 2;
			if (boolean == 1) {
				int found = 0;
				while (found == 0) {
					int check = rand() % 8;
					if (dun.hardness[X + move[check][0]][Y + move[check][1]]
							== 0
							|| dun.dun[X + move[check][0]][Y + move[check][1]]
									== 4) {
						return check;
					}
				}
			}
		}
	}

	if (Tunneling == 1) {
		if (Telepathy == 1) {
			if (Intelligence == 1) {
				int min = 0;
				for (i = 1; i < 8; ++i) {
					if (dun.Tunneling[X + move[i][0]][Y + move[i][1]]
							< dun.Tunneling[X + move[min][0]][Y + move[min][1]]) {
						min = i;
					}
				}
				return min;
			} else {
				if (X < dun.PlayerX && Y < dun.PlayerY) {
					return 7;
				} else if (X == dun.PlayerX && Y < dun.PlayerY) {
					return 6;
				} else if (X > dun.PlayerX && Y < dun.PlayerY) {
					return 5;
				} else if (X < dun.PlayerX && Y == dun.PlayerY) {
					return 4;
				} else if (X > dun.PlayerX && Y == dun.PlayerY) {
					return 3;
				} else if (X < dun.PlayerX && Y > dun.PlayerY) {
					return 2;
				} else if (X == dun.PlayerX && Y > dun.PlayerY) {
					return 1;
				} else {
					return 0;
				}
			}
		} else {
			if (Intelligence == 1) {
				if (canSee(dun) == 1) {
					int min = 0;
					for (i = 1; i < 8; ++i) {
						if (dun.Tunneling[X + move[i][0]][Y + move[i][1]]
								< dun.Tunneling[X + move[min][0]][Y
										+ move[min][1]]) {
							min = i;
						}
					}
					return min;
				} else {
					return rand() % 8;
				}
			} else {
				return rand() % 8;
			}
		}
	} else {
		if (Telepathy == 1) {
			if (Intelligence == 1) {
				int min = 0;
				for (i = 1; i < 8; ++i) {
					if (dun.nonTunneling[X + move[i][0]][Y + move[i][1]]
							< dun.nonTunneling[X + move[min][0]][Y
									+ move[min][1]]
							&& dun.hardness[X + move[i][0]][Y + move[i][1]]
									== 0) {
						min = i;
					}
				}
				return min;
			} else {
				if (X < dun.PlayerX && Y < dun.PlayerY) {
					return 7;
				} else if (X == dun.PlayerX && Y < dun.PlayerY) {
					return 6;
				} else if (X > dun.PlayerX && Y < dun.PlayerY) {
					return 5;
				} else if (X < dun.PlayerX && Y == dun.PlayerY) {
					return 4;
				} else if (X > dun.PlayerX && Y == dun.PlayerY) {
					return 3;
				} else if (X < dun.PlayerX && Y > dun.PlayerY) {
					return 2;
				} else if (X == dun.PlayerX && Y > dun.PlayerY) {
					return 1;
				} else {
					return 0;
				}
			}
		} else {
			if (Intelligence == 1) {
				if (canSee(dun) == 1) {
					int min = 0;
					for (i = 1; i < 8; ++i) {
						if (dun.nonTunneling[X + move[i][0]][Y + move[i][1]]
								< dun.nonTunneling[X + move[min][0]][Y
										+ move[min][1]]
								&& dun.hardness[X + move[i][0]][Y + move[i][1]]
										== 0) {
							min = i;
						}
					}
					return min;
				} else {
					int found = 0;
					while (found == 0) {
						int check = rand() % 8;
						if (dun.hardness[X + move[check][0]][Y + move[check][1]]
								== 0
								|| dun.dun[X + move[check][0]][Y
										+ move[check][1]] == 4) {
							return check;
						}
					}
				}
			} else {
				int found = 0;
				while (found == 0) {
					int check = rand() % 8;
					if (dun.hardness[X + move[check][0]][Y + move[check][1]]
							== 0
							|| dun.dun[X + move[check][0]][Y + move[check][1]]
									== 4) {
						return check;
					}
				}
			}
		}
	}

	return -1;
}

void monster::getLet() {
	if (num == 5) {
		printw("0");
	} else if (num == 6) {
		printw("1");
	} else if (num == 7) {
		printw("2");
	} else if (num == 8) {
		printw("3");
	} else if (num == 9) {
		printw("4");
	} else if (num == 10) {
		printw("5");
	} else if (num == 11) {
		printw("6");
	} else if (num == 12) {
		printw("7");
	} else if (num == 13) {
		printw("8");
	} else if (num == 14) {
		printw("9");
	} else if (num == 15) {
		printw("a");
	} else if (num == 16) {
		printw("b");
	} else if (num == 17) {
		printw("c");
	} else if (num == 18) {
		printw("d");
	} else if (num == 19) {
		printw("e");
	} else if (num == 20) {
		printw("f");
	}
}
