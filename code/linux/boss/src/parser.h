/* May be modified as part of BOSS, a derived work, as of 10/06/2009 */

/*  BohShell (a simple linux shell) ver v0.4
    Copyright (C) 2007-2008  Santostefano Giovanni

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/ 

/* parser
 * parser files implements the parser that divide the string
 * typed in the prompt in a string array that may contain the program
 * and the params
 */
 
 
 
 
 /*CONVENTIONS:
  * ./			run the program in the form ./program arg1 arg2 arg3
  * program name 	is a command found in /bin or other user defined directoryes
  * ,/file		file is a standard file. It's launched with the default program
  */

#ifndef _PARSER_
#define _PARSER_

#include "shinclude.h"
#include "parsertypes.h"
#include "association.h"
#include "usrdat.h"

//This is the main parser.
//Take a string and divide it in commands
//then call the executor for each command
//returns 1 if the parsing and execution is successfully done
int ParseCommands(char* text);

//parse a string and returns the type of the command, the
//number of arguments and the arguments in a argv array
struct parseresult* ParseText(char* text);

//parses ~/ as refering to a path relative to the home directory
//i.e. ~/program would be intrepreted as /home/augustus/program
int ParseHome(char* text, char* expanded);

#endif
  
