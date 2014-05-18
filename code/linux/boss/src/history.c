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

#include "history.h"

/*#define HISTORY_LEN 50

struct history
{
	char command[HISTORY_LEN][2048]; //the history array
	int pos; //command name length used to compare with the prompt
};*/

//Create and initialize an history and returns it
struct history* CreateHistory()
{
	struct history *temp=NULL;
	int i=0;
	
	temp=malloc(sizeof(struct history));
	
	if(!temp)
	{
		fprintf(stderr,"Could not allocate an history struct instance\n");
		fprintf(stderr,"Error in CreateHistory [history.c]\n");
		return NULL;
	}
	
	//initialize the history
	temp->pos=0;
	for(i=0; i<HISTORY_LEN; i++)
	{
		memset(temp->command[i],0,2048);
	}
	
	return temp;
}

//Add a command to the history
int AddCommandToHistory(struct history *h, char *command)
{
	//clear the old command if there is one
	memset(h->command[h->pos],0,2048);
	
	//copy the new command
	strcpy(h->command[h->pos],command);
	
	//update the position
	h->pos++;
	if(h->pos>=HISTORY_LEN)
	{
		h->pos=0;
	}
}

//Load the history from a file and returns it
//If it fails it returns NULL
//The file is ~/.boss_history
struct history* LoadHistoryFromFile()
{
	FILE* stream=NULL;
	char *home=NULL;
	char filename[2048];
	struct history *h=NULL;
	
	memset(filename,0,2048);
	
	h=CreateHistory();
	if(!h)
	{
		fprintf(stderr,"Warning! Could not load the history...\n");
		fprintf(stderr,"no space for allocating history!\n");
		fprintf(stderr,"Error in LoadHistoryFromFile [history.c]\n");
		return NULL;
	}
	
	
	//create the history file
	home=getenv("HOME");
	if(!home)
	{
		fprintf(stderr,"Could not take the home directory!\n");
		fprintf(stderr,"Error in LoadHistoryFromFile [history.c]\n");
		return NULL;
	}
	strcpy(filename,home);
	strcat(filename,"/.boss_history");
	
	stream=fopen(filename,"rt");
	if(!stream)
	{
		fprintf(stderr,"Could not create or open the history file ~/.boss_history!\n");
		fprintf(stderr,"Error in LoadHistoryFromFile [history.c]\n");
		free(h);
		return NULL;
	}
	
	//writing the history
	fread(h,sizeof(struct history),1,stream);
	fclose(stream);
	
	return h;
}

//Save the history to a file
//If it fails it returns -1
//The file is ~/.boss_history
int SaveHistoryToFile(struct history *h)
{
	FILE* stream=NULL;
	char *home=NULL;
	char filename[2048];
	
	memset(filename,0,2048);
	
	if(!h)
	{
		fprintf(stderr,"Warning! Could not save the history...\n");
		fprintf(stderr,"argument [struct history *h] is NULL!\n");
		fprintf(stderr,"Error in SaveHistoryToFile [history.c]\n");
		return -1;
	}
	
	
	//create the history file
	home=getenv("HOME");
	if(!home)
	{
		fprintf(stderr,"Could not take the home directory!\n");
		fprintf(stderr,"Error in SaveHistoryToFile [history.c]\n");
		return -1;
	}
	strcpy(filename,home);
	strcat(filename,"/.boss_history");
	
	stream=fopen(filename,"wt");
	if(!stream)
	{
		fprintf(stderr,"Could not create or open the history file ~/.boss_history!\n");
		fprintf(stderr,"Error in SaveHistoryToFile [history.c]\n");
		return -1;
	}
	
	//writing the history
	fwrite(h,sizeof(struct history),1,stream);
	fclose(stream);
	
	return 1;
}

//Print the history
int PrintHistory(struct history *h)
{
	int i=0, j=0, k=0, t=0;
	char c;
	
	if(!h)
	{
		return -1;
	}
	
	//loop in the history and print 20 by 20 only the commands not \0
	i=h->pos;
	
	while(j<HISTORY_LEN)
	{
		if(h->command[i][0]!='\0')
		{
			printf("%d) %s\n",k,h->command[i]);
			t++;
		}
		
		if(t>=20)
		{
			printf("\n Do you want to continue (y/n)?");
			scanf("%c",&c);
			getchar();
			if(c!='y' && c!='Y')
			{
				return 1;
			}
			
			t=0;
		}
		
		k++;
		i--;
		if(i<0) i=HISTORY_LEN-1;
		j++;
	}
	
	return 1;
}
