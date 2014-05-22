#ifndef PROCESS_H_
#define PROCESS_H_  

#define PIPE_READ 0
#define PIPE_WRITE 1

typedef enum {WAITING, DONE, FAILED} Status;
typedef enum {STDIN, FILEIN, PIPEIN} InType;
typedef enum {STDOUT, FILEOUT, PIPEOUT} OutType;

struct process_info{
   char *program;
   int argc; // 3
   char** argv; // "ls" "-l" NULL
   Status status; /* 0 = waiting  */
   InType in;
   OutType out;
   int pipe_out;
   int *pipe_in;
   /* file descriptor for redirection */
};

int fail_process(struct process_info* p, const char *a, const char *b);

int execute_single_process(struct process_info* p);
int execute_piped_process(struct process_info* p1, struct process_info* p2);

int execute_piped(struct process_info* p2);

int pipe_fd_out(struct process_info* p2, int fd);

#endif
