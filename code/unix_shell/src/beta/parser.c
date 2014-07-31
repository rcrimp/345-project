/*
 * Author: Tom, Reuben
 *
 * takes a string splits it into white space separated strings
 * creates structs from the strings
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "process.h"
     
commandCount = 0;

void mainParse(char* input, struct process_info **p){

   Special lastSymbol = NONE;
   int commandStored = false;
   unsigned int argumentCount;
   argumentCount = 0;

   /* for each word */
   char* word = strtok(input, " ");
   while (word != NULL){
      if (commandStored == false){
         if (lastSymbol == NONE || lastSymbol == PIPE){
            
            /* make new process struct for the command */
            (p[commandCount]) = malloc(sizeof(struct process_info));
            (p[commandCount])->program = (char*)malloc(sizeof(char)*(strlen(word)+1));
            strcpy((p[commandCount])->program, word);
            (p[commandCount])->argc = 1;
            (p[commandCount])->argv = (char**)malloc(sizeof(char*)*ARGMAX);
            (p[commandCount])->argv[0] = (p[commandCount])->program;
            (p[commandCount])->argv[1] = NULL;
            (p[commandCount])->status = WAITING;
            (p[commandCount])->out = STDOUT;
            if (lastSymbol == PIPE) {
               (p[commandCount])->in = PIPEIN;
               (p[commandCount])->pipe_in = &(p[commandCount-1]->pipe_out);
               (p[commandCount-1])->out = PIPEOUT;
            } else {
               (p[commandCount])->in = STDIN;
            }
            commandStored = true;
            commandCount++;
            argumentCount = 1;
         } else if (lastSymbol == REDIR_OUT){
            
         }
      } else {
         if (0 == strcmp(word, "|")){
            commandStored = false;
            lastSymbol = PIPE;
         } else if (0 == strcmp(word, "<")){
            commandStored = false;
            lastSymbol = REDIR_IN;
         } else if (0 == strcmp(word, ">")){
            commandStored = false;
            lastSymbol = REDIR_OUT;
         } else if (0 == strcmp(word, ">>")){
            commandStored = false;
            lastSymbol = APPEND_OUT;
         } else {
            /* add new arg to the argv array */
            (p[commandCount-1])->argv[argumentCount] = (char*)malloc(sizeof(char)*(strlen(word)+1));
            strcpy((p[commandCount-1])->argv[argumentCount], word);
            (p[commandCount-1])->argv[argumentCount+1] = NULL;
         
            argumentCount++;
            (p[commandCount-1])->argc = argumentCount;
         }
      }
      word = strtok(NULL, " ");
   }
   commandStored = false;
}
