#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "process.h"

int execute_process(struct process_info* p){
   int fd[2];
   char output[4096];
   pid_t cpid;

   if(pipe(fd) == -1){
      fprintf(stderr, "failed pipe");
      perror("");
      return -1;
   }
   cpid = fork();
   
   if(cpid == 0){
      if(execv(p->path, p->argv) < 0){
         fprintf(stderr, "failed exec\n");
         perror(p->path); // to fd
         return -1;
      }
   }else{
      if(cpid < 0){
         fprintf(stderr, "borked the fork\n");
         perror(""); // to fd
         return -1;
      }
      waitpid(cpid, NULL, 0);
   }
   return 0;
}

int execute_process_pipe(struct process_info* p1, struct process_info* p2){
   int fd[2];
   pid_t cpid1, cpid2;

   pipe(fd);
   cpid1 = fork();

   if(cpid1 == 0){ /* child */
      dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
      close(fd[STDOUT_FILENO]);
      if(execv(p1->path, p1->argv) < 0){
         fprintf(stderr, "failed execv\n");
         return -1;
      }
   }else{ /* main thread */
      if(cpid1 < 0){
         fprintf(stderr, "borked the fork\n");
         return -1;
      }
      cpid2=fork();
      if (cpid2==0){ /* second child */
         dup2(fd[STDIN_FILENO], STDIN_FILENO);
         close(fd[STDIN_FILENO]);
         if(execv(p2->path, p2->argv) < 0){
            fprintf(stderr, "failed execv\n");
            return -1;
         }
      } else{ /* main thread */
         if(cpid2 < 0){
            fprintf(stderr, "borked the fork\n");
            return -1;
         }
         //waitpid(cpid2, NULL, 0);
         waitpid(cpid1, NULL, 0);
      }
      //waitpid(cpid1, NULL, 0);
   }
   
   return 0;
}

int execute_array_process_pipes(struct process_info** p1){
   return -1;
}
