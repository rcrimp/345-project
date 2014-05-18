#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#include <errno.h>

#include "process.h"

#include <iostream>

using namespace std;

#define PIPE_READ 0
#define PIPE_WRITE 1

int fail_process(struct process_info* p, const char *a, const char *b) {
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
   if((cpid = fork()) == -1)
      return fail_process(p,"borked the fork\n", "");
   
   if(cpid == 0){
      dup2(fd[PIPE_WRITE], STDOUT_FILENO);
      //dup2(fd[2], STDERR_FILENO);
      close(fd[PIPE_READ]);
      if(execv(p->path, p->argv) < 0)
         return fail_process(p,"failed execv\n", p->path);
   }else{
      close(fd[PIPE_WRITE]);
      //close(fd[2]);
      p->fd = fd[PIPE_READ];
      waitpid(cpid, NULL, 0);
      p->status = DONE;
   }
   return 0;
}

int execute_process_pipe(struct process_info* p1, struct process_info* p2){
   int fd[2];
   pid_t cpid;

    if(p1->status != DONE)
      execute_process(p1);

   if(pipe(fd) == -1)
      return fail_process(p2,"failed pipe\n", "");
   if((cpid = fork()) == -1)
      return fail_process(p2,"borked the fork\n", "");
   
   if(cpid == 0){
      dup2(fd[PIPE_WRITE], STDOUT_FILENO);
      dup2(p1->fd, STDIN_FILENO);
      //dup2(fd[2], STDERR_FILENO);
      close(fd[PIPE_READ]);
      if(execv(p2->path, p2->argv) < 0)
         return fail_process(p2,"failed execv\n", p2->path);
   }else{
      close(fd[PIPE_WRITE]);
      //close(fd[2]);
      p2->fd = fd[PIPE_READ];
      waitpid(cpid, NULL, 0);
      p2->status = DONE;
   }

   return 0;
}

int execute_array_process_pipes(struct process_info** p1){
   return -1;
}

// int execute_process_pipe(struct process_info* p1, struct process_info* p2){
//    int pipe_fd[2];
//    pid_t cpid;

//    if(pipe(pipe_fd) == -1)
//       return fail_process(p1,"failed pipe\n", "");
//    if((cpid = fork()) == -1)
//       return fail_process(p1,"borked the fork\n", "");

//    if(cpid == 0){ /* child */
//       printf("In child process (pid = %d)\n", getpid());
//       dup2(pipe_fd[PIPE_WRITE], STDOUT_FILENO);
//       //close(pipe_fd[PIPE_WRITE]);
//       if(execv(p1->path, p1->argv) < 0)
//          return fail_process(p1,"failed execv\n", p1->path);
//    }else{ /* main thread */
//       if((cpid = fork()) == -1)
//          return fail_process(p2,"borked the fork\n", "");
//       if (cpid == 0){ /* second child */
//          printf("In child process (pid = %d)\n", getpid());
//          dup2(pipe_fd[PIPE_READ], STDIN_FILENO);
//          //close(pipe_fd[PIPE_OUT]);
//          if(execv(p2->path, p2->argv) < 0)
//             return fail_process(p2,"failed execv\n", p2->path);
//       } else{ /* main thread */
         
//          //cerr << cpid1;
//           //cerr << "\n";
//           //cerr << cpid2;
//           //cerr << "\n";

//          //waitpid(cpid, NULL, 0);
//       }
//       //waitpid(cpid, NULL, 0);
//    }
//    //waitpid(cpid1, NULL, 0);
//    //cerr << waitpid(cpid1, &status, WNOHANG);
    

//    // while ((wpid = wait(&status)) > 0){
//    //    printf("Exit status of %d was %d (%s)\n", (int)wpid, status,
//    //           (status > 0) ? "accept" : "reject");
//    // }

//    return 0;
// }
