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
   char out[4096];
   // char out2[4096];

   char *temp1[] = {"ls", "-l",  NULL};
   char *temp2[] = {"grep", "cpp",  NULL};
   char *temp3[] = {"sort", "-r", NULL};
   
   process_info *p1 = (process_info*)malloc(sizeof(process_info));
   p1->path = "/bin/ls";
   p1->argc = 3;
   p1->argv = temp1;
   p1->status = WAITING;

   process_info *p2 = (process_info*)malloc(sizeof(process_info));
   p2->path = "/bin/grep";
   p2->argc = 2;
   p2->argv = temp2;
   p2->status = WAITING;

   process_info *p3 = (process_info*)malloc(sizeof(process_info));
   p3->path = "/usr/bin/sort";
   p3->argc = 2;
   p3->argv = temp3;
   p3->status = WAITING;

   //execute_process(p1);
   // read(p1->fd, out, sizeof(out));
   // cout << out << endl;
   
   execute_process_pipe(p1, p2);
   //read(p2->fd, out, sizeof(out));
   //cout << out << endl;
    
   execute_process_pipe(p2, p3);
   // read(p1->fd, out, sizeof(out));
   // cout << out << endl;
   // read(p2->fd, out, sizeof(out));
   // cout << out << endl;
   read(p3->fd, out, sizeof(out));
   cout << out << endl;

   cout << p1->fd << endl;
   cout << p2->fd << endl;
   cout << p3->fd << endl;

   free(p1);
   free(p2);
   free(p3);
   return 0;
}
