/*
 * Save-Load.cpp
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include "Dungeon.h"
#include "Save-Load.h"

void save(dungeon dun) {
	FILE* fp;
	char *homes = strdup(getenv("HOME"));
	char *paths = (char *) malloc(sizeof(homes) + 17);
	strcpy(paths, homes);
	strcat(paths, "/.rlg327/dungeon");
	fp = fopen(paths, "wb");
	if (!fp) {
		perror(paths);
		return;
	}
	char title[12] = { 'R', 'L', 'G', '3', '2', '7', '-', 'F', '2', '0', '1',
			'8' };
	fwrite(&title, sizeof(title), 1, fp);
	unsigned int ver[1] = { 0 };
	fwrite(&ver, sizeof(ver), 1, fp);
	int size = 0;
	int i;
	for (i = 0; i < 9; ++i) {
		if (dun.rooms[0][i] != -1) {
			size++;
		}
	}
	unsigned int fileSize[1];
	fileSize[0] = (1701 + (size * 4));
	fwrite(&fileSize, sizeof(fileSize), 1, fp);
	char x[2] = {(char) dun.rooms[0][0], (char) dun.rooms[1][0] };
	fwrite(&x, sizeof(x), 1, fp);
	int j;
	for (i = 0; i < 21; ++i) {
		for (j = 0; j < 80; j++) {
			char h[1];
			h[0] = dun.hardness[j][i];
			fputc(h[0], fp);
		}
	}
	int cur = 0;
	while (dun.rooms[0][cur] != -1) {
		for (int i = 0; i < 4; ++i) {
			char put;
			if (i == 0) {
				put = dun.rooms[0][cur] - dun.rooms[2][cur];
				fputc(put, fp);
			}
			if (i == 1) {
				put = dun.rooms[1][cur] - dun.rooms[3][cur];
				fputc(put, fp);
			}
			if (i == 2) {
				put = (2 * dun.rooms[2][cur]) + 1;
				fputc(put, fp);
			}
			if (i == 3) {
				put = (2 * dun.rooms[3][cur]) + 1;
				fputc(put, fp);
			}
		}
		cur++;
	}
	fclose(fp);
	free(homes);
	free(paths);
}

dungeon load() {
	FILE* fp;
	char *homel = strdup(getenv("HOME"));
	char *pathl = (char *) malloc(sizeof(homel) + 17);
	strcpy(pathl, homel);
	strcat(pathl, "/.rlg327/dungeon");
	fp = fopen(pathl, "rb");
	if (!fp) {
		perror(pathl);
	}
	char title[12];
	fread(&title, sizeof(title), 1, fp);
	int i;
	unsigned int ver[1];
	fread(&ver, sizeof(ver), 1, fp);
	unsigned int fileSize[1];
	fread(&fileSize, sizeof(fileSize), 1, fp);
	unsigned char x[2];
	fread(&x, sizeof(x), 1, fp);
	unsigned char hard[80][21];
	int j;
	for (i = 0; i < 21; i++) {
		for (j = 0; j < 80; j++) {
			unsigned char h[1];
			fread(&h, sizeof(h), 1, fp);
			hard[j][i] = h[0];
		}
	}
	char *loc = (char *) malloc(fileSize[0] - 1701);
	int n;
	int Total = ((fileSize[0] - 1701) / (sizeof(char[4])));
	dungeon dun;
	for (i = 0; i < 9; ++i) {
		for (n = 0; n < 4; ++n) {
			dun.rooms[n][(i)] = -1;
		}
	}
	for (i = 0; i < Total; ++i) {
		for (n = 0; n < 4; ++n) {
			char first[1];
			fread(&first, sizeof(first), 1, fp);
			loc[(i * 4) + n] = first[0];
		}
		dun.rooms[0][(i)] = loc[(i * 4)];
		dun.rooms[1][(i)] = loc[(i * 4) + 1];
		dun.rooms[2][(i)] = loc[(i * 4) + 2];
		dun.rooms[3][(i)] = loc[(i * 4) + 3];
	}
	for (i = 0; i < 21; ++i) {
		for (j = 0; j < 80; j++) {
			dun.hardness[j][i] = hard[j][i];
		}
	}
	for (i = 0; i < 21; ++i) {
		for (j = 0; j < 80; j++) {
			if (hard[j][i] == 255) {
				dun.dun[j][i] = 0;
			} else if (hard[j][i] == 0) {
				int n;
				for (n = 0; n < Total; ++n) {
					if (j >= loc[(n * 4)]
							&& j < (loc[(n * 4)] + loc[(n * 4) + 2])
							&& i >= loc[(n * 4) + 1]
							&& i < (loc[(n * 4) + 1] + loc[(n * 4) + 3])) {
						dun.dun[j][i] = 2;
						break;
					} else {
						dun.dun[j][i] = 3;
					}
				}
			} else {
				dun.dun[j][i] = 1;
			}
		}
	}
	dun.dun[x[0]][x[1]] = 4;
	dun.PlayerX = x[0];
	dun.PlayerY = x[1];
	dun.drawdun();
	fclose(fp);
	free(homel);
	free(pathl);
	free(loc);
	return dun;
}
