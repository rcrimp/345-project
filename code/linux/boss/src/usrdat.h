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

/* usrdat.h
 * This file contains the prototypes of the functions
 * that gets user data
 */

#ifndef _USRDAT_
#define _USRDAT_

#include "shinclude.h"

struct userdat
{
	char* username; //full user name
	char* loginname; //login name of the user
	char* home; //home directory
	char* shell; //me :)
	char* terminal; //system terminal
	pid_t shellpid; //the shell pid
	uid_t uid; //user id
	gid_t gid; //group id
};

//this function returns the user data in a populated 
//userdat structure instance
//This function is executed at the beginning to get 
//info of the user 
struct userdat* GetUserData();

//Free the user data structure
void FreeUserData(struct userdat* data);

//Print a formatted user data structure
void PrintUserData(struct userdat* data);

#endif
