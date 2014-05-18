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
 
#include "builtin.h"

//Creates an instance of a builtin command
struct builtincommand* CreateBuiltInCommand(char* name, int (*handler)(char*))
{
	struct builtincommand* cmd=NULL;
	char* p=NULL;
	int i=0;
	
	cmd=malloc(sizeof(struct builtincommand));
	
	if(!cmd)
	{
		fprintf(stderr,"Could not allocate memory for builtin command allocation\n");
		fprintf(stderr,"Implementation in CreateBuiltInCommand in [builtin.c]\n");
		return NULL;
	}
	
	//command character count
	p=name;
	
	while(*p!=' ' && *p!='\n' && *p!='\0')
	{
		p++;
		i++;
	}
	i++; //space for \0
	
	//allocating space for the builtin name
	cmd->command=malloc(sizeof(char)*i);
	if(!cmd->command)
	{
		free(cmd);
		cmd=NULL;
		fprintf(stderr,"Could not allocate memory for builtin command name\n");
		fprintf(stderr,"Implementation in CreateBuiltInCommand in [builtin.c]\n");
		return NULL;
	}
	
	
	strncpy(cmd->command,name,i);
	cmd->len=i;
	cmd->bif=handler;
	
	return cmd;
}

//Add a builtin command to a list
struct builtinlist* AddNodeToBuiltinList(struct builtinlist *list, char *name, int (*handler)(char*))
{
	struct builtincommand *com=NULL;
	struct builtinlist *temp=NULL;
	
	com=CreateBuiltInCommand(name, handler);
	
	if(!com)
	{
		return NULL;
	}
	
	temp=malloc(sizeof(struct builtinlist));
	if(!temp)
	{
		fprintf(stderr,"Could not allocate memory for builtin list node\n");
		fprintf(stderr,"Implementation in AddNodeToBuiltinList in [builtin.c]\n");
		
		//deallocating command
		if( com->command ) { free(com->command); com->command=NULL; }
		com->bif=NULL;
		free(com);
		com=NULL;
		
		return NULL;
	}
	
	temp->cmd=com;
	temp->next=list;
	list=temp;
	
	return list;
	
	
}

//Remove a builtin command from a list
//You must pass the pointer to the pointer of previous element of the list
//if the node to remove is the list head you have to pass the list head
//and 1 to isHead param.
//This function completely deallocate the builtin command prev->next
int RemoveNodeFromBuiltinList(struct builtinlist **prev, int isHead)
{
	struct builtincommand *com=NULL;
	struct builtinlist *temp=NULL;
	
	if(!(*prev))
	{
		fprintf(stderr,"Error in memory pointed by prev\n");
		fprintf(stderr,"Implementation in RemoveNodeFromBuiltinList in [builtin.c]\n");
		return -1;
	}
	
	if(isHead==1) //I don't use if(isHead) to be more clear
	{
		if(!(*prev)->next)
		{
			temp=*prev; //element to remove
			*prev=NULL;
		}
		else
		{
			temp=(*prev)->next; //element to remove
			(*prev)->next=((*prev)->next)->next;
		}
		
		//deallocating temp->cmd
		if((temp->cmd)->command){ free( (temp->cmd)->command ); (temp->cmd)->command=NULL; } 
		(temp->cmd)->bif=NULL;
		if(temp->cmd){ free(temp->cmd); temp->cmd=NULL; }
		temp->next=NULL;
		if(temp){ free(temp); temp=NULL; }
		
		return 1;
	}
	else
	{
		if(!(*prev)->next)
		{
			fprintf(stderr,"Error in memory pointed by prev->next\n");
			fprintf(stderr,"Implementation in RemoveNodeFromBuiltinList in [builtin.c]\n");
			return -1;
		}
	
		temp=(*prev)->next; //element to remove
		
		(*prev)->next=((*prev)->next)->next;
		
		//deallocating temp->cmd
		if((temp->cmd)->command){ free( (temp->cmd)->command ); (temp->cmd)->command=NULL; } 
		(temp->cmd)->bif=NULL;
		if(temp->cmd){ free(temp->cmd); temp->cmd=NULL; }
		temp->next=NULL;
		if(temp){ free(temp); temp=NULL; }
		
		return 1;
	}
	
}

//Find and execute a builtin command if it's in the list
int FindNExecuteBuiltinCommand(struct builtinlist *list, char *com)
{
	struct builtinlist *temp=NULL;
	struct builtincommand *bincmd=NULL;
		
	temp=list;
	
	while(temp)
	{
		bincmd=temp->cmd; //get the command
		
		if( strncmp(com,bincmd->command, bincmd->len)==0 )
		{
			if(bincmd->bif)
			{
				return bincmd->bif(com);
			}
			else
			{
				fprintf(stderr,"Builtin function handler missing!\n");
				fprintf(stderr,"Implementation in FindNExecuteBuiltinCommand [builtin.c]\n");
				return BUILTIN_CMD_ERROR;
			}
		}
		
		temp=temp->next;
	}
	
	return BUILTIN_CMD_NOT_FOUND;
}


/* COMMANDS */

//This is the cd command
int BuiltinCD(char *text)
{
	char *p=NULL;
	int res=0;
	char directory[2048];
	struct userdat* data = NULL; //We need the home directory path
	
	//cd command execution
	//For first we verify if a command cd is
	//inserted. If we found a cd we execute it
	//with the first argument, excluding pipe and 
	//redirections...
	

	

	//Zeroing out the strings that in future may contain the cwd
	memset(directory,0,sizeof(char)*2048);
	
	p=text;
	while(*p==' ') p++;
	
	if((*p=='c' && *(p+1)=='d') && (*(p+2)==' ' || *(p+2)=='\0'))
	{
		p+=2;
		
		//space removal
		while(*p==' ')p++;
		
		if(*p=='/') //absolute path
		{
			res=chdir(p); //change the directory

			if(res==-1) //some error occurred
			{
				if(errno==ENOTDIR)
				{
					fprintf(stderr,"Directory not Specified\n");
				}
				else if(errno==EACCES)
				{
					fprintf(stderr,"Access Denied!\n");
				}
				else
				{
					fprintf(stderr,"Can't change directory\n");
					fprintf(stderr,"ERROR: %s\n",strerror(errno));
				}
				
				return -1; //change dir failed
			}
			
			return 1; //change dir success
		}

		else if (*p == '~' &&(*(p + 1) == '/' || *(p + 1) == '\0'))
		{
			p += 1; //get past ~, we need the /

			// get the home directory
			data = GetUserData();

			//make the absolute path to the directory (hopefully it exists...)
			strcat(directory, data->home); //add the home directory path
			strcat(directory, p); //add the path relative to the home directory
			

			res = chdir(directory);

			//release the usr data
			if(data) { free(data); data = NULL;}
			
			if (res == -1) //something went wrong
			{
				if (errno == ENOTDIR)
				{
					fprintf(stderr, "Directory not specified\n");
				}
				else if(errno==EACCES)
				{
					fprintf(stderr,"Access Denied!\n");
				}
				else
				{
					fprintf(stderr,"Can't change directory\n");				
					fprintf(stderr,"ERROR: %s\n",strerror(errno));
				}
				
				return -1; //error
			}

			return 1; //success
		}

		else if (*p == '\0')
		{
			//get home directory
			data = GetUserData();

			//change directory to home dir
			strcat(directory, data->home);

			//free usr data
			if (data) { free(data); data = NULL; }

			res = chdir(directory);

			 if (res == -1) //something went wrong
                        {
                                if (errno == ENOTDIR)
                                {
                                        fprintf(stderr, "Directory not specified\n");
                                }
                                else if(errno==EACCES)
                                {
                                        fprintf(stderr,"Access Denied!\n");
                                }
                                else
                                {
                                        fprintf(stderr,"Can't change directory\n");
                                        fprintf(stderr,"ERROR: %s\n",strerror(errno));
                                }

                                return -1; //error
                        }
			return 1; // success
		}

		else
		{
			//get the absolute path of the cwd
			if(!getcwd(directory,1024))
			{
				if(errno==ERANGE && !getcwd(directory,2000))
				{
					//ERROR
					fprintf(stderr,"Can't change directory\n");
					fprintf(stderr,"Could not obtain cwd\n");
					fprintf(stderr,"buffer too short\n");
					fprintf(stderr,"Implementation in ParseCommands in [parse.c]\n");
					return -1;
				}
				else if(errno==EACCES)
				{
					fprintf(stderr,"Can't change directory\n");
					fprintf(stderr,"Access Denied in cwd path\n");
					fprintf(stderr,"Implementation in ParseCommands in [parse.c]\n");
					return -1;
				}
			}
			
			//directory now contains the cwd absolute path
			//Now we concat the cwd path to the relative path 
			//from cwd contained in [char* p]
			strcat(directory,"/");
			strcat(directory,p);
			
			res=chdir(directory); //change the directory
			
			if(res==-1) //some error occurred
			{
				if(errno==ENOTDIR)
				{
					fprintf(stderr,"Directory not Specified\n");
				}
				else if(errno==EACCES)
				{
					fprintf(stderr,"Access Denied!\n");
				}
				else
				{
					fprintf(stderr,"Can't change directory\n");
					fprintf(stderr,"ERROR: %s\n",strerror(errno));
				}
				
				return -1; //change dir failed
			}
			
			return 1; //change dir success
			
			
		}
		
	} /*END IF CD COMMAND*/
	
	return 0; //wrong command
}

//This is the exit command
int BuiltinEXIT(char *text)
{
	//Verifing the exit command
	if(strcmp(text,"exit")==0)
	{
		return EXITCODE;
	}
	
	return BUILTIN_CMD_NOT_FOUND;
}


