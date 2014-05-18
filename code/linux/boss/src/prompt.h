/* Maybe modified as part of BOSS, a derived work, as of 10/06/2009 */

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

/* prompt
 * prompt files contains the prompt printing implementation
 */

#ifndef _PROMPT_
#define _PROMPT_

#include "shinclude.h"
#include "usrdat.h"

//This procedure get the prompt format string from the configuration file
// .bohshell_conf located in the home directory of the user
//if the file does not exist or the prompt string is not found
//-1 is returned. 
//The string that will contain the prompt format must be preallocated
//
//The format of the configuration is
//...
//@start prompt
//{u[i][g] c}
//@end prompt
int GetPromptFormatFromConfigFile(char *pformat);

//This procedure print the prompt on the user's screen
//The arg printpath is an unsigned int. If you pass 1
//in the prompt is printed also the pwd.
void PrintPrompt(struct userdat *data, unsigned int ppath, char *pformat);

//Get the prompt string.
//From this string the commands are executed
//The string passed as argument must be allocated
int GetPromptCommand(char *line);

//Find a command or an application
//based on a part of a command for autocompletition
//returns -1 if nothing is found
int AutoComplete(char *line);

#endif
