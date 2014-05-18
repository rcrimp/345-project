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

/* history
 * history files are used for the implementation of the shell history
 * The history take the list of the command used.
 * You can show the history or call a command that is an history entry
 * $ ?? is the command that show the history
 * $ ?[number] call the [number] command in the history list
 */

#ifndef _HISTORY_
#define _HISTORY_

#include "shinclude.h"

#define HISTORY_LEN 50

struct history
{
	char command[HISTORY_LEN][2048]; //the history array
	int pos; //command name length used to compare with the prompt
};

//Create and initialize an history and returns it
struct history* CreateHistory();

//Add a command to the history
int AddCommandToHistory(struct history *h, char *command);

//Load the history from a file and returns it
//If it fails it returns NULL
//The file is ~/.bohsh_history
struct history* LoadHistoryFromFile();

//Save the history to a file
//If it fails it returns -1
//The file is ~/.bohsh_history
int SaveHistoryToFile(struct history *h);

//Print the history
int PrintHistory(struct history *h);

#endif
