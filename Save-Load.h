/*
 * Save-Load.h
 *
 *  Created on: Oct 20, 2018
 *      Author: mitchellsinclair
 */

#ifndef SAVE_LOAD_H_
#define SAVE_LOAD_H_

#include "Dungeon.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>

void save(dungeon dun);
dungeon load();


#endif /* SAVE_LOAD_H_ */
