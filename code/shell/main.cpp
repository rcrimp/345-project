#include <stdio.h>
#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <iostream>

#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
#include <errno.h>

#include <string.h>
#include <string>

#include "process.h"
#include "parser.h"

#define BUFFLEN 4096

using namespace std;

int main(void){
   unsigned int i;
   int j;
   char buff[BUFFLEN];
   memset(buff, '\0', sizeof(char)*BUFFLEN );
   process_info *p[128];

   // /*
   //   STDIN_FILENO  0
   //   STDOUT_FILENO 1
   //   STDERR_FILENO 2
   // */
   
   // char *arg1[] = {"ls", "-l",  NULL};
   // process_info *p1 = (process_info*)malloc(sizeof(process_info));
   // p1->program = "ls";
   // p1->argc = 2;
   // p1->argv = arg1;
   // p1->status = WAITING;

   // char *arg2[] = {"grep", "cpp",  NULL};
   // process_info *p2 = (process_info*)malloc(sizeof(process_info));
   // p2->program = "grep";
   // p2->argc = 2;
   // p2->argv = arg2;
   // p2->status = WAITING;
   // p2->pipe_in = &p1->pipe_out;

   // char *arg3[] = {"sort", "-r", NULL};
   // process_info *p3 = (process_info*)malloc(sizeof(process_info));
   // p3->program = "sort";
   // p3->argc = 2;
   // p3->argv = arg3;
   // p3->status = WAITING;
   // p3->pipe_in = &p2->pipe_out;
   
   // /* open file */
   // FILE *fp = fopen("test.txt", "w");
   // /* execute process */
   // execute_single_process(p1);
   // /* store the outpute in a temp buffer */
   // read(p1->pipe_out, temp, sizeof(temp));
   // /* write to the file from the buffer */
   // fprintf(fp, "%s", temp);

   // execute_single_process(p1);
   // execute_piped(p2);
   //execute_piped(p3);

   // read(p2->pipe_out, buff, sizeof(buff));
   // cout << buff;
   
   // //write(fileno(fp), temp, sizeof(temp));
   // read(p3->pipe_out, temp, sizeof(temp));
   // cout << temp;
   
   while (true){  
      std::string input;
      //resetting arrays
      for (i = 0; i < 128; i++){
         // commandArray[i] = "";
         // for (j = 0; j < 256; j++){
         //    argumentArray[i][j] = "";
         // }
         pipeArray[i] = "";
      }
      commandCount = 0;
      
      cout << "prompt $ ";
      getline(cin, input);
      mainParse(input, p);
      //printData();

      for(i = 0; i < commandCount; i++){
         if(execute_single_process(p[i]) != -1){
            read(p[i]->pipe_out, buff, sizeof(char)*BUFFLEN );
            cout << buff << endl;
            memset(buff, '\0', sizeof(char)*BUFFLEN );
         }

         for(j=1; j < p[i]->argc; j++){
            free(p[i]->argv[j]);
            //fprintf(stderr, "argv[%d] = %s\n", j, p[i]->argv[j]);
         }
         
         free(p[i]->program);
         free(p[i]->argv);
         free(p[i]);
      }
   }

   // fclose(fp);
    // free(p1);
    // free(p2);
    // free(p3);
   return 0;
}
