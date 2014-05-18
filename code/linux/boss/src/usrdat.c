/* Maybe modified as part of BOSS, a derived work, as of 10/06/2009 */

/*  BOHSHELL (a simple linux shell) ver v0.4
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

#include "usrdat.h"

//this function returns the user data in a populated 
//userdat structure instance
//This function is executed at the beginning to get 
//info of the user 
struct userdat* GetUserData()
{
	//allocating the structure
	struct userdat* data=NULL;
	data=malloc(sizeof(struct userdat));
	
	if(!data)
	{
		fprintf(stderr,"Could not allocate userdata structure instance\n");
		fprintf(stderr,"Program failed!\n");
		fprintf(stderr,"Error in GetUserData function [usrdat.c]");
		exit(EXIT_FAILURE);
	}
	
	//getting the envvar
	data->username=getenv("USER");
	data->loginname=getenv("LOGNAME");
	data->home=getenv("HOME");
	data->shell=getenv("SHELL");
	data->terminal=getenv("TERM");
	data->shellpid=getpid();
	data->uid=getuid();
	data->gid=getgid();
	
	return data;
}


//Free the user data structure
void FreeUserData(struct userdat* data)
{
	if(data->username){free(data->username); data->username=NULL; }
	if(data->loginname){ free(data->loginname); data->loginname=NULL; }
	if(data->home){ free(data->home); data->home=NULL; }
	if(data->terminal){ free(data->terminal); data->terminal=NULL; }
	if(data->shell){ free(data->shell); data->shell=NULL; }
}


//Print a formatted user data structure
void PrintUserData(struct userdat* data)
{
	printf("-------------------------------------------\n");
	if(data->username) printf("| User name is %s\n",data->username);
	if(data->loginname) printf("| Login name is %s\n",data->loginname);
	if(data->uid) printf("| User ID is %d\n",data->uid);
	if(data->gid) printf("| Group ID is %d\n",data->gid);
	if(data->home) printf("| Home directory is %s\n",data->home);
	printf("|------------------------------------------\n");
	if(data->terminal) printf("| Terminal is %s\n",data->terminal);
	if(data->shell) printf("| Default Shell is %s\n",data->shell);
	if(data->shellpid) printf("| Process Shell PID is %d\n",data->shellpid);
	printf("-------------------------------------------\n");
}
