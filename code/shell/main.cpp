#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>

#include <string.h>
#include <string>

#include "process.h"
#include "parser.h"

using namespace std;

int main(void){
   // char temp[4096];

   // char *arg1[] = {"ls", "-l",  NULL};
   // char *arg2[] = {"grep", "cpp",  NULL};
   // char *arg3[] = {"sort", "-r", NULL};
   
   // process_info *p1 = (process_info*)malloc(sizeof(process_info));
   // p1->path = "ls";
   // p1->argc = 3;
   // p1->argv = arg1;
   // p1->status = WAITING;

   // process_info *p2 = (process_info*)malloc(sizeof(process_info));
   // p2->path = "grep";
   // p2->argc = 2;
   // p2->argv = arg2;
   // p2->status = WAITING;

   // process_info *p3 = (process_info*)malloc(sizeof(process_info));
   // p3->path = "sort";
   // p3->argc = 2;
   // p3->argv = arg3;
   // p3->status = WAITING;

   while (true){
      std::string input;
      // resetting arrays
      for (unsigned int i = 0; i < 128; i++){
         commandArray[i] = "";
         for (unsigned int j = 0; j < 256; j++){
            argumentArray[i][j] = "";
         }
         pipeArray[i] = "";
      }
      commandCount = 0;
      
      getline(cin, input);
      mainParse(input);
      printData();
   }

   // free(p1);
   // free(p2);
   // free(p3);
   return 0;
}
