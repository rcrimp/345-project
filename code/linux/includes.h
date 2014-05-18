#ifndef INCLUDES_H
#define INCLUDES_H

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include <stdio.h>
#include <iostream>

#define INLEN 2048
#define ARGLEN 256

struct process_info
{
   char *program; //program or command name
   int argc; //number of argumemts
   char **argv; //list of arguments
};

#endif
