#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>

#include <string.h>
#include <string.h>

#include "process.h"

#define BUFFLEN 4096

#define true 1
#define false 0 

int main(void){
   unsigned int i;
   int j;
   char buff[BUFFLEN];
   char input[BUFFLEN];
   memset(buff, '\0', sizeof(char)*BUFFLEN );
   struct process_info *p[128];

   // /*
   //   STDIN_FILENO  0
   //   STDOUT_FILENO 1
   while (true){  
      commandCount = 0;

      printf("prompt $ ");

      scanf("%4095s", &input);

      mainParse(input, p);

      if(commandCount > 0){
         //printData();

         for(i = 0; i < commandCount; i++){
            if (execute_piped(p[i]) == -1){
               fprintf(stderr, "error\n");
               break;
            }
         }

         if(p[i-1]->status == DONE){
            read(p[commandCount-1]->pipe_out, buff, sizeof(char)*BUFFLEN );
            printf(buff);
            memset(buff, '\0', sizeof(char)*BUFFLEN);
         }

         for(i = 0; i < commandCount; i++){
            for(j=1; j < p[i]->argc; j++){
               free(p[i]->argv[j]);
            }
            free(p[i]->program);
            free(p[i]->argv);
            free(p[i]);
         }
      }
   }
   return 0;
}
