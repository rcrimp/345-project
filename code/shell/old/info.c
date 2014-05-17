#ifndef __INFO__
#define __INFO__

#include "preincludes.h"

struct info{
   char* user;
   char* login;
   char* home;
   char* shell;
   char* term;
   pid_t pid;
   uid_t uid;
   gid_t gid;
};

struct info* get_shell_info();

void free_shell_info(struct info* things);

#endif
