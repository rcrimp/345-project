#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
     
using namespace std;
     
void printData();
void mainParse(string input);

#define ARGMAX 128
     
//std::string commandArray[128]; //list of commands
//std::string argumentArray[128][256]; //list of arguments for each command
std::string pipeArray[128]; //list of pipe symbols that fit betweent the commands
unsigned int commandCount = 0;
     
void mainParse(std::string input, process_info **p){
   unsigned int argumentCount = 0;
   bool commandStored = false;
   
   std::istringstream iss(input);
   std::string word;
         
   while (iss >> word){
      if ((word == "|" || word == "<" || word == ">"
           || word == "<<" || word == ">>") && commandStored == true){
         pipeArray[commandCount] = word;
         commandStored = false;
      } else if (commandStored == false){
         (p[commandCount]) = (process_info*)malloc(sizeof(process_info));
         
         (p[commandCount])->program = (char*)malloc(sizeof(char)*(word.length()+1));
         strcpy((p[commandCount])->program, (char*)word.c_str());
            
         (p[commandCount])->argc = 1;
         
         (p[commandCount])->argv = (char**)malloc(sizeof(char*)*ARGMAX);
         (p[commandCount])->argv[0] = (p[commandCount])->program;
         (p[commandCount])->argv[1] = NULL;

         (p[commandCount])->status = WAITING;
         
         argumentCount = 1; /* need this and argc ?? */
         commandStored = true;
         commandCount++;    
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
   commandStored = false;
}
     
// void printData(){
     
//    std::string output = "";
//    for (unsigned int i = 0; i < 128; i++){
//       if (commandArray[i] != ""){
//          cout << commandArray[i] << endl;
//       }
//       for (unsigned int j = 0; j < 256; j++){
//          if (argumentArray[i][j] != ""){
//             cout << argumentArray[i][j] + "-arg ";
//          }
//       }
//    }
// }

