#include <stdio.h> /* for the scanf() call */
#include <stdlib.h> /* for the system() call */

int main() {
  char string[80];

  for(;;){
     /*scanf("%79s", string);*/
    system("ls -la");
  }
  return 0;
}
