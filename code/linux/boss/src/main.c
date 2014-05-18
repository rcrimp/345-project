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

#include "shinclude.h"
#include "usrdat.h"
#include "prompt.h"
#include "parser.h"
#include "launcher.h"
#include "builtin.h"
#include "history.h"
#include "association.h"

int main()
{
	int i=0;
	struct userdat* data=NULL;
	int status=0;
	char line[2048];
	char pformat[2048];
	struct builtinlist *binlist=NULL;
	struct history *h=NULL;
	
	
	
	char *prog=NULL;
	
	//prompt format finding
	memset(pformat,0,2048);
	GetPromptFormatFromConfigFile(pformat);
	
	
	
	data=GetUserData();
	
	//Builtin commands init
	binlist=AddNodeToBuiltinList(binlist, "cd ",BuiltinCD );
	if(!binlist)
	{
		printf("ERROR cd\n");
	}
	
	binlist=AddNodeToBuiltinList(binlist, "cd",BuiltinCD ); //This is a little hack to get "cd" by itself to take us
        if(!binlist)						//the home directory
        {
                printf("ERROR cd\\0\n");
        }

	binlist=AddNodeToBuiltinList(binlist, "exit",BuiltinEXIT );
	if(!binlist)
	{
		printf("ERROR exit\n");
	}
		
	
	//Get the history from the home bohshell_history file
	h=LoadHistoryFromFile();
	if(!h)
	{
		//If we have no history saved
		//we create the history
		h=CreateHistory();
	}
	

	printf("             BOSS (Beginner Open Source Shell) Version 0.0.2\n");
	printf("             Developed by Augustus Maxim <entropy51423@users.sourceforge.net>\n");
	printf("             http://bossshell.sourceforge.net\n\n");
	PrintUserData(data);

	while(1)
	{
		PrintPrompt(data,1,pformat);
		memset(line,0,DIRLEN);
		status=GetPromptCommand(line);
		
				
		if(status==2)
		{
			continue;
		}
		
		//history showing and history command call
		if(line[0]=='?')
		{
			if(line[1]=='?')
			{
				//add the command to the history
				PrintHistory(h);
				continue;
			}
			else
			{
				//Let's call the i-th command in the history
				//status will contain the nuber of comand
				status=atoi(line+1);
				status=h->pos-status;
				if(status>HISTORY_LEN)
				{
					status+=HISTORY_LEN;
				}
				
				if(status>=0 && status<HISTORY_LEN && h->command[status][0]!='\0')
				{
					memset(line,0,2048);
					strcpy(line,h->command[status]);
				}
			}
		}
		
		//adding the command to the history
		//if it's not an history call
		if(line[0]!='?')
		{
			//add the command to the history
			status=AddCommandToHistory(h, line);
		}
		
		//builtin command execution
		status=FindNExecuteBuiltinCommand(binlist, line);
		if(status==BUILTIN_CMD_NOT_FOUND || status==BUILTIN_CMD_ERROR)
		{
			status=ParseCommands(line);
		}
		
		//exit from the shell
		if(status==EXITCODE)
		{
			//save history to a file
			SaveHistoryToFile(h);
			
			//FreeUserData(data);
			if(data){ free(data); data=NULL; }
			
			status=RemoveNodeFromBuiltinList(&binlist, 1);
			if(status<0)
			{
				printf("ERROR remove exit\n");
			}
	
			status=RemoveNodeFromBuiltinList(&binlist, 1);
			if(status<0)
			{
				printf("ERROR remove cd\n");
			}
			exit(EXIT_SUCCESS);
		}
	}
	
	return 0;
}
