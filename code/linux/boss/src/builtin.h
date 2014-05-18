/* Maybe modified as part of BOSS, a derived work, as of 10/06/2009 */

/*  BohShell (a simple linux shell) v0.4
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

/* builtin
 * builtin files are used for the implementation of the shell builtin commands
 */

#ifndef _BUILTIN_
#define _BUILTIN_

#include "shinclude.h"
#include "usrdat.h"

#define BUILTIN_CMD_NOT_FOUND -5678
#define BUILTIN_CMD_ERROR -5679

struct builtincommand
{
	char *command; //the command name
	int len; //command name length used to compare with the prompt
	int (*bif)(char *prompt); //pointer to a builtin function
};

struct builtinlist
{
	struct builtincommand *cmd;
	struct builtinlist *next;
};

/*PRIVATE*/
//Creates an instance of a builtin command
struct builtincommand* CreateBuiltInCommand(char *name, int (*handler)(char*));


/*PUBLIC*/
//Add a builtin command to a list
struct builtinlist* AddNodeToBuiltinList(struct builtinlist *list, char *name, int (*handler)(char*));

//Remove a builtin command from a list
//You must pass the pointer to the pointer of previous element of the list
//if the node to remove is the list head you have to pass the list head
//and 1 to isHead param.
//This function completely deallocate the builtin command prev->next
int RemoveNodeFromBuiltinList(struct builtinlist **prev, int isHead);

//Find and execute a builtin command if it's in the list
int FindNExecuteBuiltinCommand(struct builtinlist *list, char *com);


/* COMMANDS */

//This is the cd command
int BuiltinCD(char *text);

//This is the exit command
int BuiltinEXIT(char *text);

//This is the show history command
int BuiltinSHOWHISTORY(char* text);

#endif

