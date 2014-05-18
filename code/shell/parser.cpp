#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "parser.h"
 
using namespace std;
 
std::string commandArray[128]; //list of commands
std::string argumentArray[128][256]; //list of arguments for each command
std::string pipeArray[128]; //list of pipe symbols that fit betweent the commands
int commandCount = 0;
int argumentCount = 0;
 
//splits between pipe symbols
void mainParse(std::string input){
   
   bool pipe = false;
   for (unsigned int i = 0; i < input.length(); i++){
     
      if (input[i] == '|'){
         pipe = true;
         pipeArray[commandCount] = input[i];
         parseCommand(input.substr(0, i));
         mainParse(input.substr(i +1));
      }
      if (input[i] == '<' || input[i] == '>'){
         pipe = true;
         if (input[i] == input[i +1]){
            pipeArray[commandCount] = input[i] + input[i +1];
            parseCommand(input.substr(0, i +1));
            mainParse(input.substr(i +2));
         } else {
            pipeArray[commandCount] = input[i];
            parseCommand(input.substr(0, i));
            mainParse(input.substr(i +1));
         }
      }
      if (i == input.length() -1 && pipe == false){
         parseCommand(input);
      }
   }
}
//separates arguments from command and saves command as string
void parseCommand(std::string command){
   
   unsigned int count = 0;
   while (command[0] == ' ' && command.length() > 1){
      command = command.substr(1);
   } //remove whitespace
   if (command == " ") return;
   while (count < command.length() && command[count] != ' '){
      if (command[count] == '\\') count++;
      count++;
   }
   commandArray[commandCount] = command.substr(0, count);
   commandCount++;
   parseArguments(command.substr(count));
}
//splits arguments up and saves as array
void parseArguments(std::string arguments){
 
   argumentCount = 0;
   while (arguments[arguments.length() -1] == ' '){
      arguments = arguments.substr(0, arguments.length() -2);
   } //remove whitespace
   while (arguments.length() > 0){  
      unsigned int count = 0;
      while (arguments[0] == ' ' && arguments.length() > 1){
         arguments = arguments.substr(1);
      } //remove whitespace
      while (count < arguments.length() && arguments[count] != ' '){
         if (arguments[count] == '\\') count++;
         count++;
      }
      argumentArray[commandCount][argumentCount] = arguments.substr(0, count);
      arguments = arguments.substr(count);
      argumentCount++;
   }
}
 
//prints the command and piping data
void printData(){
   std::string output = "";
   for (unsigned int i = 0; i < 128; i++){
      if (commandArray[i] != ""){
         cout << commandArray[i] + " ";
      }
      for (unsigned int j = 0; j < 256; j++){
         if (argumentArray[i][j] != ""){
            cout << argumentArray[i][j] + " ";
         }
      }
   }
}
