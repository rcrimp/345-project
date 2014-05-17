#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include "process.h"

int fail_process(struct process_info* p, const char *a, char *b) {
   fprintf(stderr,a);
   perror(b);
   p->status=FAILED;
   return -1;
}


int execute_process(struct process_info* p){
   int fd[2];
   pid_t cpid;

   if(pipe(fd) == -1)
      return fail_process(p,"failed pipe\n", "");
   if((cpid = fork()) < 0)
      return fail_process(p,"borked the fork\n", "");
   
   if(cpid == 0){
      dup2(fd[1], STDOUT_FILENO);
      close(fd[0]);
      if(execv(p->path, p->argv) < 0)
         return fail_process(p,"failed execv\n", p->path);
   }else{
      close(fd[1]);
      read(fd[0], p->output, sizeof(p->output));
      waitpid(cpid, NULL, 0);
      p->status = DONE;
   }
   return 0;
}

// int execute_process_pipe(struct process_info* p1, struct process_info* p2){
//    int fd[2];
//    pid_t cpid1, cpid2;

//    if(pipe(fd) == -1)
//       return fail_process(p1,"failed pipe\n", "");
//    if((cpid1 = fork()) < 0)
//       return fail_process(p1,"borked the fork\n", "");

//    if(cpid1 == 0){ /* child */
//       dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
//       close(fd[STDOUT_FILENO]);
//       if(execv(p1->path, p1->argv) < 0)
//          return fail_process(p1,"failed execv\n", p1->path);
//    }else{ /* main thread */
//       if((cpid2 = fork()) < 0)
//          return fail_process(p2,"borked the fork\n", "");
//       if (cpid2==0){ /* second child */
//          dup2(fd[STDIN_FILENO], STDIN_FILENO);
//          close(fd[STDIN_FILENO]);
//          if(execv(p2->path, p2->argv) < 0)
//             return fail_process(p2,"failed execv\n", p2->path);
//       } else{ /* main thread */
//          waitpid(cpid1, NULL, 0);
//       }
//    }
   
//    return 0;
// }

int execute_process_pipe(struct process_info* p1, struct process_info* p2){
      int fd[2];
      pid_t cpid1, cpid2;

      if(pipe(fd) == -1)
         return fail_process(p1,"failed pipe\n", "");
      if((cpid1 = fork()) < 0)
         return fail_process(p1,"borked the fork\n", "");

      if(cpid1 == 0){ /* child */
         dup2(fd[STDOUT_FILENO], STDOUT_FILENO);
         close(fd[STDOUT_FILENO]);
         execute_process(p1);
      }else{ /* main thread */
         if((cpid2 = fork()) < 0)
            return fail_process(p2,"borked the fork\n", "");
         if (cpid2==0){ /* second child */
            dup2(fd[STDIN_FILENO], STDIN_FILENO);
            close(fd[STDIN_FILENO]);
            execute_process(p2);
         } else{ /* main thread */
            waitpid(cpid1, NULL, 0);
         }
      }
   
      return 0;
}

int execute_array_process_pipes(struct process_info** p1){
   return -1;
}
