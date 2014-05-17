#ifndef PROCESS_H_
#define PROCESS_H_  

typedef enum {WAITING, DONE, FAILED} Status;

struct process_info{
   char *path; // "/bin/ls"
   int argc; // 3
   char** argv; // "ls" "-l" NULL
   Status status; /* 0 = waiting  */
   char output[4096];
   /* file descriptor for redirection */
};


int execute_process(struct process_info* p);
int execute_process_pipe(struct process_info* p1, struct process_info* p2);
int execute_array_process_pipes(struct process_info** p1);

#endif
