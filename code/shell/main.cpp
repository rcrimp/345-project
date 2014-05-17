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

#define READ_END 0
#define WRITE_END 1

int main(void){

   char *temp[] = {"ls", NULL};  
   process_info *p1 = new process_info {0, "2/bin/ls", 3, temp};

   char *temp2[] = {"grep", "cpp",  NULL};
   process_info *p2 = new process_info{0, "/bin/grep", 2, temp2};
   
   execute_process(p1);
   
   execute_process_pipe(p1, p2);

   return 0;

}
