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

#include "parser.h"

//this is a private function used by ParseCommands to
//see, in the case we have a ,/ statement, if the arg passed
//is an extension... like [ $ ,/c ] that means *.c
//we fill a string with all the files .c in the cwd and return 1
int GetAllFilesByExtension(char *filenames, char *text)
{
	char cwd[2048];
	char *p=NULL, *j=NULL;
	DIR *dirfile=NULL; //directory handler
	struct dirent *direntry=NULL; //directory entry
	int status=0;
	
	memset(cwd,0,2048);
	p=text;
	if(text[0]==',' && text[1]=='/')
	{
		p=text+2;
	}
	
	
	//verifyng if we have only extensions
	j=p;
	while(*j!='\0' && *j!='\n')
	{
		if(*j=='.')
		{
			//this is not the form ,/extension
			return -1;
		}
		
		j++;
	}
		
	
	//taking the current working directory
	getcwd(cwd,2047);
	
	dirfile=opendir(cwd);
		
	if(!dirfile)
	{
		fprintf(stderr,"Could not open the cwd! \n");
		fprintf(stderr,"Error in GetAllFilesByExtension function in [parser.c]\n");
		return -1;
	}
	
	while(direntry=readdir(dirfile)) //while there are entry in the directory
	{
		status=0;
		j=direntry->d_name;
		while(*j!='\0' && *j!='\n')
		{
			if(*j=='.')
			{
				//this is not the form ,/extension
				status=1;
				break;
			}
		
			j++;
		}
		
		
		//not found extension
		if(!status)
		{
			continue;
		}

		//extension found
		j++;
		
		if( strcmp(j,p)==0 )
		{
			strcat(filenames, " ");
			strcat(filenames, direntry->d_name);
		}
		
		
	}
	
	closedir(dirfile);
	return 1;
	
}

//This is the main parser.
//Take a string and divide it in commands
//then call the executor for each command
//returns 1 if the parsing and execution is successfully done
int ParseCommands(char *text)
{
	struct parseresult *result[2];
	int status=0, res=0,i=0;
	int numcommands=0;
	int pipe=0;
	char command[1024];
	char *p=NULL, *j=NULL;
	const char *p2=NULL;
	char directory[2048];
	char com1[2048],com2[2048]; //commands in the subdivision after a pipe is found
	char compositecom[2048]; //the composite command is the result of the elaboration of ,/command
	char *compositeprog=NULL; //the program associated to the extension in the case of ,/
	char compositelist[100000]; //containing all the file with a specific extension in cwd
	int isextension=0; //tell us if the prompt is an extension or a file list
	
	
	//Zeroing out the strings that contains the piped commands
	memset(com1,0,2048);
	memset(com2,0,2048);
	memset(compositecom,0,2048);
	memset(compositelist,0,100000);

	//Zeroing out the strings that in future may contain the cwd
	memset(directory,0,sizeof(char)*2048);

	//invalid text is passed
	if(text[0]=='\n' || text[0]=='\0') return -1;
	
	//counting commands
	numcommands=1;
	pipe=0;
	p=text;
	j=text;
	
	//case we have a ,/file we have to change the command
	if(text[0]==',' && text[1]=='/')
	{
		j+=2;
		status=GetAllFilesByExtension(compositelist, j);
		if(status==1 && compositelist[0] != '\0')
		{
			j=compositelist;
		}
		
		compositeprog=FindFileAssociation(j);
		if(!compositeprog)
		{
			fprintf(stderr,"Could not find an associated program for this file\n");
			fprintf(stderr,"Error in ParseCommands [parser.c]\n");
			return -1;
		}
		
		//coping the program name and params
		p=compositeprog;
		while(*p!='\0')
		{
			if(*p=='\n') *p='\0';
			p++;
		}
		strcpy(compositecom,compositeprog);
		//add a space
		strcat(compositecom," ");
		//add the files as parameters without ,/
		strcat(compositecom,j);
		
		//assigning to p the new command memlocation
		p=compositecom;
		//set the new text to the composite command
		text=compositecom;
				
		//free the compositeprog
		free(compositeprog);
		compositeprog=NULL;
	}
	
	//beginning space removal
	while(*p==' ')p++;
	
	/////////////////////////////////////////////////////////////////////////////////////////
	//Qua ci va il controllo sulla lista dei builtin
	/////////////////////////////////////////////////////////////////////////////////////////
	
	
	/*PARSING NORMAL COMMANDS*/
	numcommands=1;
	while(*p!='\n' && *p!='\0')
	{
		if(*p=='|')
		{
			numcommands=2;
			pipe=1;
		}
		p++;
	}
	
	//only one program must be launched
	if(numcommands==1)
	{
		p=text;

		//space removal
		while(*p==' ')p++;
		
		//parsing and execution of the program
		result[0]=ParseText(p);
		
		if(result[0])
		{
			//execution
			ExecuteFGSingleProcess(result[0]);
			
			//cleaning of the parseresult struct instance
			for(i=0; i<result[0]->argc; i++)
			{
				if(result[0]->argv[i]){ free(result[0]->argv[i]); result[0]->argv[i]=NULL; }
			}
			free(result[0]->argv);
			result[0]->argv=NULL;
			free(result[0]);
			result[0]=NULL;
		}
		
	}
	else if(numcommands==2 && pipe==1)
	{
		p=text;
		i=0;
		
		while(*p!='\n' && *p!='\0')
		{
			//space removal
			while(*p==' ')p++;
			
			//command1
			while(*p!='|')
			{
				com1[i]=*p;
				i++;
				p++;
			}
			
			//command 2
			i=0;
			//space removal and pipe removal to prevent ex: ls | ||| | more
			while(*p==' ' || *p=='|')p++;
			while(*p!='\n' && *p!='\0')
			{
				com2[i]=*p;
				i++;
				p++;
			}
		}
		
		//parsing the commands
		result[0]=ParseText(com1);
		result[1]=ParseText(com2);
		
		if(result[0] && result[1])
		{
			ExecuteFGPipeProcess(result[0],result[1]);
			
			//cleaning of the parseresult struct instance
			for(i=0; i<result[0]->argc; i++)
			{
				if(result[0]->argv[i]){ free(result[0]->argv[i]); result[0]->argv[i]=NULL; }
			}
			free(result[0]->argv);
			result[0]->argv=NULL;
			free(result[0]);
			result[0]=NULL;
			
			for(i=0; i<result[1]->argc; i++)
			{
				if(result[1]->argv[i]){ free(result[1]->argv[i]); result[1]->argv[i]=NULL; }
			}
			free(result[1]->argv);
			result[1]->argv=NULL;
			free(result[1]);
			result[1]=NULL;
		}
		
	}
	
}

//parse a string and returns the type of the command, the
//number of arguments and the arguments in a argv array
struct parseresult* ParseText(char* text)
{
	char* p=NULL;
	int i,j,k;
	struct parseresult* res=NULL;
	struct userdat* data = NULL;
		
	res=malloc(sizeof(struct parseresult));
	
	if(!res)
	{
		fprintf(stderr,"Could not allocate memory for program parsing\n");
		fprintf(stderr,"Error in ParseText function [parse.c]");
		return NULL;
	}
	memset(res,0,sizeof(struct parseresult));
	res->redirtype=NONE;
	res->isappend=0;
	
	//calculating the params number
	p=text;
	i=0;
	k=0; //redirection filename char index
	res->argc=0;
	while(*p!='\n' && *p!='\0')
	{

		//Verifyng stdin redirection
		if(*p=='<')
		{
			res->redirtype=STDIN;
			
			if(*(p+1)=='<')
			{
				res->isappend=1;
				p+=2;  //passing over the redirect
			}
			else
			{
				res->isappend=0;
				p++; //passing over the redirect
			}
						
			//find filename for redirection
			while(*p==' ') p++; //jumping spaces
			while(*p!='\n' && *p!='\0') //finding filename
			{
				res->redfilename[k]=*p;
				k++;
				p++;
			}
			
			break;
		}
		
		//Verifyng stdout redirection
		if(*p=='>')
		{
			res->redirtype=STDOUT;
			
			if(*(p+1)=='>')
			{
				res->isappend=1;
				p+=2;  //passing over the redirect
			}
			else
			{
				res->isappend=0;
				p++;  //passing over the redirect
			}
			
			//find filename for redirection
			while(*p==' ') p++; //jumping spaces
			while(*p!='\n' && *p!='\0') //finding filename
			{
				res->redfilename[k]=*p;
				k++;
				p++;
			}
			
			break;
		}
		
		//Verifyng stderr redirection
		if(*p=='#')
		{
			res->redirtype=STDERR;
			
			if(*(p+1)=='#')
			{
				res->isappend=1;
				p+=2;  //passing over the redirect
			}
			else
			{
				res->isappend=0;
				p++;  //passing over the redirect
			}
			
			//find filename for redirection
			while(*p==' ') p++; //jumping spaces
			while(*p!='\n' && *p!='\0') //finding filename
			{
				res->redfilename[k]=*p;
				k++;
				p++;
			}
			
			break;
		}
		
		if(*p=='\\' && *(p+1)==' ')
		{
			// if the situation is \[space] we substract an argument
			res->argc--;
		}
		
		if(*p==' ')
		{
			res->argc++;
			
			//space removal
			while(*p==' ') p++;
		}
		
		
		p++;
	}
	
	//even the command is a parameter so we add 1 to the parameters counter
	// ls -l .-a  are not 2 args but 3 args because ls is an argument too!
	res->argc++; 
	
	//no arguments!!! ERROR!!!
	if(res->argc==0)
	{
		free(res);
		res=NULL;
		return NULL;
	}	
	
	//begin from verifyng the type of 
	//the command
	if(text[0]=='.' && text[1]=='/')
	{
		res->result=PROGRAM;
	}
	else
	{
		res->result=COMMAND;
	}
	
	
	
	//parameter subdivision
	//The parameter subdivision stops if the parser encounter
	//a redirection token.
	//If a redirection is found, all the params after the redirect
	//file are lost!
	
	//allocating space for argv
	res->argv=malloc(sizeof(char*)*res->argc+1);
	res->argv[res->argc]=(char*)0;
	//fixed arguments length
	for(i=0; i<res->argc; i++)
	{
		res->argv[i]=malloc(sizeof(char)*ARGLEN);
		memset(res->argv[i],0,sizeof(char)*ARGLEN);
	}
	
	p=text;
	if(res->result==PROGRAM)
	{
		p+=2; //passing over the ./
	}

	i=0; //status
	j=0; //actual parameter
	k=0; //char index at actual parameter
	while(*p!='\n' && *p!='\0' && *p!='<' && *p!='>' && *p!='#')
	{
		if(*p=='<' || *p=='>' || *p=='#')
		{
			//redirection file parsing
			break;
		}
		
		//jumping spaces
		while(*p==' ') p++;
		
		//writing the Jth argument 
		while(*p!='\n' && *p!='\0' && *p!=' ')
		{
			
			//in this situation we add the [space] to the parameter and skip to the next char
			//EX: ./go\ gio.out   ->>>  go gio.out
			if(*p=='\\' && *(p+1)==' ')
			{
				p++;
				res->argv[j][k]=*p;
				k++;
				p++;
				continue;
			}
			
			
			if(*p=='<' || *p=='>' || *p=='#' )
			{
				p--; //prevent from the next p++;
				break;
			}
			
			if (k == 0 && *p == '~') //we don't want /~/ because that wouldn't make any sense
			{
				if(!data) data = GetUserData(); //if we don't already have the data
				
				if(data) //make sure we got it
				{
					strcat(res->argv[j], data->home);
					while(res->argv[j][k] != '\0') k++; //skip past what we added 
					p++;
					continue;
				}
			}
				

			res->argv[j][k]=*p;
			k++;
			p++;
		}
		
		k=0;
		j++;
		p++;
	}

	if(data) {free(data); data = NULL;} //If there was data free it
	
	return res;
}

