#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "process.h"

//extern std::string commandArray[128]; //list of commands
//extern std::string argumentArray[128][256]; //list of arguments for each command
extern std::string pipeArray[128]; //list of pipe symbols that fit betweent the commands
extern unsigned int commandCount;

void printData();
void mainParse(std::string input, process_info **p);

#endif
