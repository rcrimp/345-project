#include <string.h>

#ifndef PARSER_H_
#define PARSER_H_

#define COMLEN
#define ARGMAX
#define ARGLEN

extern std::string commandArray[128];
extern std::string argumentArray[128][256];
extern std::string pipeArray[128];
extern int commandCount;
extern int argumentCount;

void printData();
void parseArguments(std::string arguments);
void parseCommand(std::string command);
void mainParse(std::string input);

#endif
