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

#include "prompt.h"

//This procedure get the prompt format string from the configuration file
// .bohshell_conf located in the home directory of the user
//if the file does not exist or the prompt string is not found
//-1 is returned. 
//The string that will contain the prompt format must be preallocated with 2048 chars
//
//The format of the configuration is
//...
//@start prompt
//{u[i][g] c}
//@end prompt
int GetPromptFormatFromConfigFile(char *pformat)
{
	FILE *stream=NULL;
	char *temp=NULL;//[2048];
	char conffile[2048];
	char *home=NULL;
	char *p=NULL, *j=NULL;
	int n=0;
		
	if(!pformat)
	{
		fprintf(stderr,"ERROR!!! pfromat parameter is NULL\n");
		fprintf(stderr,"implemetation in GetPromptFormatFromConfigFile [prompt.c]\n");
		return -1;
	}
	
	//get the home directory
	home=getenv("HOME");
	strcpy(conffile,home);
	//concat with the filename of the configuration file
	strcat(conffile,"/.boss_conf");
	
	//open the configuration file
	stream=fopen(conffile,"r");
	
	if(!stream)
	{
		fprintf(stderr,"Warning! configuration file %s not found\n",conffile);
		fprintf(stderr,"Prompt is now switching to the default mode!\n");
		return -1;
	}
	
	//finding the first tag @start bohsh config
	while(1)
	{
		free(temp);
		temp=NULL;
		if(getline(&temp,&n,stream)==-1)
		{
			fprintf(stderr,"Warning!reached the end of configuration file\n");
			fprintf(stderr,"whithout finding tags\n");
			fprintf(stderr,"Prompt is now switching to the default mode!\n");
			fclose(stream);
			return -1;
		}
		p=temp;
		
		//space removal
		while(*p==' ')p++;
		
		if(*p=='@')
		{
			break;
		}
	}
	
	//verifying
	
	j=p;
	while(*j!='\0')
	{
		if(*j=='\n')
		{
			*j='\0';
			break;
		}
		j++;
	}
		
	if(strcmp(p,"@start boss config\0")!=0)
	{
		printf("%d\n",strcmp(p,"@start boss config"));
		fprintf(stderr,"Warning!error in the parsing of configuration file\n");
		fprintf(stderr,"could not find @start boss config\n");
		fprintf(stderr,"Prompt is now switching to the default mode!\n");
		fclose(stream);
		return -1;
	}
	
	
	
	//finding the tag @start prompt
	while(1)
	{
		//free(temp);printf("HELO\n");
		//temp=NULL;
		
		if(getline(&temp,&n,stream)==-1)
		{
			fprintf(stderr,"Warning!reached the end of configuration file\n");
			fprintf(stderr,"whithout finding tags\n");
			fprintf(stderr,"Prompt is now switching to the default mode!\n");
			fclose(stream);
			return -1;
		}
		
		p=temp;
		
		
		//space removal
		while(*p==' ')p++;
		
		//transform \n to \0
		j=p;
		while(*j!='\0')
		{
			if(*j=='\n')
			{
				*j='\0';
				break;
			}
			j++;
		}
		
		if(strcmp(p,"@start prompt")==0)
		{
			break;
		}
	}//end while @start prompt
	
	
	//finding the prompt format
	//this is the first line !='@' or !='#' or !='\n'
	while(1)
	{
		//free(temp);
		//temp=NULL;
		if(getline(&temp,&n,stream)==-1)
		{
			fprintf(stderr,"Warning!reached the end of configuration file\n");
			fprintf(stderr,"whithout finding tags\n");
			fprintf(stderr,"Prompt is now switching to the default mode!\n");
			fclose(stream);
			free(temp);
			return -1;
		}
		
		p=temp;
		
		//space removal
		while(*p==' ')p++;
		
		//transform \n to \0
		j=p;
		while(*j!='\0')
		{
			if(*j=='\n')
			{
				*j='\0';
				break;
			}
			j++;
		}
		
		if(*p!='@' && *p!='#' && *p!='\n' && *p!='\0'  )
		{
			//This is my format!!!!!
			
			strcpy(pformat,p);
			printf("Prompt Format found! %s\n\n\n",pformat);
			
			//WARNING: IF I CLOSE THE FILE THE PROGRAMS GIVE.ME A SEGMENTATION FAULT
			fclose(stream);
			
			return 1;
		}
	}//end while @start prompt
}

//This procedure print the prompt on the user's screen
//The arg printpath is an unsigned int. If you pass 1
//in the prompt is printed also the pwd.
void PrintPrompt(struct userdat *data, unsigned int ppath, char *pformat)
{
	//{giovanni[500|100]}$

	char *p=NULL;
	char directory[1024];
	char element[1024];
	memset(directory,0,1024);
	memset(element,0,1024);

	getcwd(directory,1024);
	
	if(!pformat || pformat[0]=='\0') //if we have no prompt format defined we switch in the default mode
	{
		printf("{%s[%d|%d]",data->loginname,data->uid,data->gid);
	
		if(ppath==1)
		{
			printf(" %s",directory);
		}
	
		printf("}");
	
		
	}
	else
	{
		p=pformat;
		
		while(*p!='\0')
		{
			if(*p=='u')
			{
				//I must write &data->loginname[0]
				//otherwise if I only write data->loginname the
				//program gives me a segmentation fault
				printf("%s",&data->loginname[0]);
			}
			else if(*p=='d')
			{
				printf("no impl");
			}
			else if(*p=='i')
			{
				printf("%d\0",*&(data->uid));
			}
			else if(*p=='g')
			{
				printf("%d\0",data->gid);
			}
			else if(*p=='c')
			{
				//I must write &directory[0]
				//otherwise if I only write data->loginname the
				//program gives me a segmentation fault
				printf("%s",&directory[0]);
			}
			else
			{
				printf("%c",*p);
			}
			p++;
		}
	}
	
	if(data->uid==0 && data->gid==0)
	{
		printf("# "); //stay tuned "root"
	}
	else
	{
		printf("$ "); //user :-(
	}
	
}


//Get the prompt string.
//From this string the commands are executed
//The string passed as argument must be allocated
int GetPromptCommand(char *line)
{
	char c='a';
	int i=0,j=0;
	int result=0;
	
	while(1)
	{
		c=getchar();
		
		if(i>2047)
		{
			fprintf(stderr,"Command too long\n");
			return -1;
		}
		if(c=='\n' || c=='\0')
		{
			result=-1;
			
			for(j=0; j<i; j++)
			{
				if(line[j]=='^')
				{
					line[j]='\0';
					line[j+1]='\0';
			
					result=AutoComplete(line);
					if(result<0)
					{
						continue;
					}
					else
					{
						//printf("\n%s",line);
						//i=j-1;
						return 2;
					}
				}
			}
			
			if(result<0)
			{
				line[i]='\0';
				line[i+1]='\0';
				return 1;
			}
		}
		
		line[i]=c;
		i++;
	}
}

//Find a command or an application
//based on a part of a command for autocompletition
//returns -1 if nothing is found
int AutoComplete(char *line)
{
	char *p=NULL;
	char *t=NULL;
	
	DIR *dirfile=NULL; //directory handler
	struct dirent *direntry=NULL; //directory entry
	
	char directory[DIRLEN];
	char *pathlist;
	
	int len=0;
	int i=0;
	
	//Zeroing out the strings
	memset(directory,0,DIRLEN);
	
	p=line;
	len=strlen(p)-2; //taking the length of the last command minus the ./
	
	while(*p==' ') p++; //space removal
	
	if( (*p=='.' || *p==',') && *(p+1)=='/')
	{
		p+=2;
		while(*p==' ') p++; //space removal
		
		//open the current working directory
		getcwd(directory,DIRLEN-1);
		dirfile=opendir(directory);
		
		if(!dirfile)
		{
			fprintf(stderr,"Could not open the cwd! \n");
			fprintf(stderr,"Error in AutoComplete function in [prompt.c]\n");
			return -1;
		}
		
		while(direntry=readdir(dirfile)) //while there are entry in the directory
		{
			if(strncmp(p,direntry->d_name,len)==0)
			{
				
				//file found!!!!!!!!
				/*memset((line+2),0,DIRLEN-2);
				
				strcpy(line+2,direntry->d_name);*/
				printf("--> %s\n",direntry->d_name);	
			}
		}
		
		closedir(dirfile);
		return 1;
	}
	else
	{
		len+=2; //whe have no ./ to remove
		pathlist=getenv("PATH");
		
		printf("Searching path: %s\n",pathlist);
		
		//finding the commands in the direcories of the user
		t=pathlist;
		while(*t==' ') t++;
		while(*t!='.' && *t!='\0')
		{
			i=0;
			memset(directory,0,DIRLEN);
			
			while(*t!=':' && *t!='.' && *t!='\0')
			{
				directory[i]=*t;
				t++;
				i++;
			}
			t++; //step to the other directory name
			
			if(i>0)
			{
				dirfile=opendir(directory);
		
				if(!dirfile)
				{
					fprintf(stderr,"Could not open the directory %s! \n",directory);
					continue;
				}
		
				while(direntry=readdir(dirfile)) //while there are entry in the directory
				{
					if(strncmp(p,direntry->d_name,len)==0)
					{
				
						//file found!!!!!!!!
						printf("--> %s\n",direntry->d_name);	
					}
				}
		
				closedir(dirfile);
			}
		}
		
		
		
		return 1;
	}
}
