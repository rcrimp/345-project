#ifndef PROCESS_H_
#define PROCESS_H_  

#define PIPE_READ 0
#define PIPE_WRITE 1

typedef enum {WAITING, DONE, FAILED} Status;

struct process_info{
   char *path; // "/bin/ls"
   int argc; // 3
   char** argv; // "ls" "-l" NULL
   Status status; /* 0 = waiting  */
   int fd_out;
   int fd_in;
   /* file descriptor for redirection */
};

int fail_process(struct process_info* p, const char *a, const char *b);

int execute_single_process(struct process_info* p);
int execute_piped_process(struct process_info* p1, struct process_info* p2);

int pipe_fd_in(int fd, struct process_info* p2);
int pipe_fd_out(struct process_info* p2, int fd);

#endif
