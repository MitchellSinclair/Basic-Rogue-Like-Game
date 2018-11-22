/*
 * Parser.h
 *
 *  Created on: Oct 28, 2018
 *      Author: mitchellsinclair
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <iostream>
#include "parMon.h"
#include "parItm.h"
#include <vector>

class Parser{
public:
	Parser();
	~Parser();
	std::vector<parMon> parseMons();
	std::vector<parItm> parseItms();
};


#endif /* PARSER_H_ */
