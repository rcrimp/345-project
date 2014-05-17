#ifndef PROCESS_H_
#define PROCESS_H_  

struct process_info{
   int status; /* 0 = successfull */
   char *path; // "/bin/ls"
   int argc; // 3
   char** argv; // "ls" "-l" NULL
   /* file descriptor for redirection */
};


int execute_process(struct process_info* p);
int execute_process_pipe(struct process_info* p1, struct process_info* p2);
int execute_array_process_pipes(struct process_info** p1);

#endif
