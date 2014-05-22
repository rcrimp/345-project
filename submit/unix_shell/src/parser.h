#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include "process.h"

#define ARGMAX 128

typedef enum {NONE, PIPE, REDIR_OUT, REDIR_IN, APPEND_OUT, BG} Special;

extern unsigned int commandCount;

void mainParse(std::string input, process_info **p);

#endif
