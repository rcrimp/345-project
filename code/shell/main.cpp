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

#include "process.h"

using namespace std;

int main(void){
   char *temp1[] = {"ls", "-l",  NULL};
   char *temp2[] = {"grep", "cpp",  NULL};
   
   //process_info *p1 = new process_info{"/bin/ls", 3, temp1, WAITING};
   process_info *p1 = (process_info*)malloc(sizeof(process_info));
   p1->path = "/bin/ls";
   p1->argc = 3;
   p1->argv = temp1;
   p1->status = WAITING;

   process_info *p2 = new process_info{"/bin/grep", 2, temp2, WAITING};




   
   execute_process(p1);
   //cout << p1->output;
   
   execute_process_pipe(p1, p2);

   free(p1);
   //free(p2);
   return 0;
}
