#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parser.h"
#include "process.h"
     
using namespace std;
     
unsigned int commandCount = 0;
     
void mainParse(std::string input, process_info **p){

   Special lastSymbol = NONE;
   unsigned int argumentCount = 0;
   bool commandStored = false;
   
   std::istringstream iss(input);
   std::string word;
         
   while (iss >> word){
      if (commandStored == false){
         if (lastSymbol == NONE || lastSymbol == PIPE){
            
            /* make new process struct for the command */
            (p[commandCount]) = (process_info*)malloc(sizeof(process_info));
            (p[commandCount])->program = (char*)malloc(sizeof(char)*(word.length()+1));
            strcpy((p[commandCount])->program, (char*)word.c_str());
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
         if (word == "|"){
            commandStored = false;
            lastSymbol = PIPE;
         } else if (word == "<"){
            commandStored = false;
            lastSymbol = REDIR_IN;
         } else if (word == ">"){
            commandStored = false;
            lastSymbol = REDIR_OUT;
         } else if (word == ">>"){
            commandStored = false;
            lastSymbol = APPEND_OUT;
         } else {
            //cout << "unused arg: " << word << endl; 
            //argumentArray[commandCount][argumentCount] = word;
            //(p[commandCount-1])->argv[argumentCount] = (char*)word.c_str();
            //(p[commandCount-1])->argv[argumentCount+1] = NULL;

            (p[commandCount-1])->argv[argumentCount] = (char*)malloc(sizeof(char)*(word.length()+1));
            strcpy((p[commandCount-1])->argv[argumentCount], (char*)word.c_str());
            (p[commandCount-1])->argv[argumentCount+1] = NULL;
         
            argumentCount++;
            (p[commandCount-1])->argc = argumentCount;
         }
      }
   }
   commandStored = false;
}
