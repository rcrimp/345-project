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

/*launcher
 * is the system that fork and exec a new process with a new program.
 * It executes a new program in many ways... such as in pipe, with redirection,
 * in a future implementation also in background.
 */

#include "launcher.h"

//This function launch a single program in foreground
int ExecuteFGSingleProcess(struct parseresult* pr)
{
	pid_t process=0;
	int fd;
	char command[2048];  //contains the full pathname of the program
	
	memset(command,0, sizeof(char)*2048);
	
	//first of all we fork a process
	if((process=fork())<0)
	{
		fprintf(stderr,"Error on process child creation, %s\n",strerror(errno));
		fprintf(stderr,"Error in ExecuteFGSingleProcess in [launcher.c]\n");
		return -1;
	}
	
	if(process==0) /*child*/
	{
		//process execution
		
		//redirection mode
		if(pr->redirtype!=NONE)
		{
			if(pr->redirtype==STDIN)
			{
				if(pr->isappend)
				{
					fd=open(pr->redfilename,O_RDWR | O_APPEND,0x777);
					dup2(fd,STDIN_FILENO);
					close(fd);
				}
				else
				{
					fd=open(pr->redfilename,O_RDWR| O_CREAT,0x777);
					dup2(fd,STDIN_FILENO);
					close(fd);
				}
			}
			else if(pr->redirtype==STDOUT)
			{
				if(pr->isappend)
				{
					fd=open(pr->redfilename,O_RDWR | O_APPEND,0x777);
					dup2(fd,STDOUT_FILENO);
					close(fd);
				}
				else
				{
					fd=open(pr->redfilename,O_RDWR|O_CREAT,0x777);
					dup2(fd,STDOUT_FILENO);
					close(fd);
				}
			}
			else if(pr->redirtype==STDERR)
			{
				if(pr->isappend)
				{
					fd=open(pr->redfilename,O_RDWR | O_APPEND,0x777);
					dup2(fd,STDERR_FILENO);
					close(fd);
				}
				else
				{
					fd=open(pr->redfilename,O_RDWR | O_CREAT,0x777);
					dup2(fd,STDERR_FILENO);
					close(fd);
				}
			}
		}/*end of redirection*/
		
		
		//execution by file
		if(pr->result==PROGRAM)
		{
			//obtaining the full program path
			if(*(pr->argv[0])=='/')
			{
				strcpy(command,pr->argv[0]);
			}
			else
			{
				//get the absolute path of the cwd
				if(!getcwd(command,1024))
				{
					if(errno==ERANGE && !getcwd(command,2000))
					{
					//ERROR
						fprintf(stderr,"error in directory\n");
						fprintf(stderr,"Could not obtain cwd\n");
						fprintf(stderr,"buffer too short\n");
						fprintf(stderr,"Implementation in ExecuteFGSingleProcess in [launcher.c]\n");
						return -1;
					}
					else if(errno==EACCES)
					{
						fprintf(stderr,"error in directory\n");
						fprintf(stderr,"Access Denied in cwd path\n");
						fprintf(stderr,"Implementation in ExecuteFGSingleProcess in [laucher.c]\n");
						return -1;
					}
				}
				
				//directory now contains the cwd absolute path
				//Now we concat the cwd path to the relative path 
				//from cwd contained in [char* p]
				strcat(command,"/\0");
				strcat(command,pr->argv[0]);
			}
	
			if((execv(command,pr->argv))<0)
			{
				fprintf(stderr,"Can't execute command\n");
				fprintf(stderr,"Implementation in ExecuteFGSingleProcess in [laucher.c]\n");
				exit(EXIT_FAILURE);	
			}
			
		} /*END PROGRAM*/




		//execution by command
		if(pr->result==COMMAND)
		{
			strcpy(command,pr->argv[0]);
			if((execvp(command,pr->argv))<0)
			{
				fprintf(stderr,"Can't execute command\n");
				fprintf(stderr,"Implementation in ExecuteFGSingleProcess in [laucher.c]\n");
				exit(EXIT_FAILURE);	
			}
		} /*END COMMAND*/
		
		
	}
	else /*parent*/
	{
		waitpid(process,0,0);
		return 1;
	}
}


//This function launch two programs linked by a pipe
int ExecuteFGPipeProcess(struct parseresult* pr1, struct parseresult* pr2)
{
	pid_t process=0;
	char command[2048];  //contains the full pathname of the program
	
	int pipein[2], pipeout[2];
	int ret=0;
	
	memset(command,0, sizeof(char)*2048);
	
	if((ret=pipe(pipein)))
	{
		fprintf(stderr,"Error on input pipe creation, %s\n",strerror(errno));
		fprintf(stderr,"Error in ExecuteFGPipeProcess in [launcher.c]\n");
		return -1;
	}
	
	if((ret=pipe(pipeout)))
	{
		fprintf(stderr,"Error on output pipe creation, %s\n",strerror(errno));
		fprintf(stderr,"Error in ExecuteFGPipeProcess in [launcher.c]\n");
		return -1;
	}
	
	
	//first of all we fork a process
	if((process=fork())<0)
	{
		fprintf(stderr,"Error on process child creation, %s\n",strerror(errno));
		fprintf(stderr,"Error in ExecuteFGPipeProcess in [launcher.c]\n");
		return -1;
	}
	
	if(process==0) /*child*/
	{	
		
		close ( pipein [1]); 
		dup2 ( pipein [0] , STDIN_FILENO );
		close ( pipeout [0]);
		dup2 ( pipeout [1] , STDOUT_FILENO );

		//execution by file
		if(pr1->result==PROGRAM)
		{
			//obtaining the full program path
			if(*(pr1->argv[0])=='/')
			{
				strcpy(command,pr1->argv[0]);
			}
			else
			{
				//get the absolute path of the cwd
				if(!getcwd(command,1024))
				{
					if(errno==ERANGE && !getcwd(command,2000))
					{
					//ERROR
						fprintf(stderr,"error in directory\n");
						fprintf(stderr,"Could not obtain cwd\n");
						fprintf(stderr,"buffer too short\n");
						fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [launcher.c]\n");
						return -1;
					}
					else if(errno==EACCES)
					{
						fprintf(stderr,"error in directory\n");
						fprintf(stderr,"Access Denied in cwd path\n");
						fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [laucher.c]\n");
						return -1;
					}
				}
				
				//directory now contains the cwd absolute path
				//Now we concat the cwd path to the relative path 
				//from cwd contained in [char* p]
				strcat(command,"/\0");
				strcat(command,pr1->argv[0]);
			}
	
			if((execv(command,pr1->argv))<0)
			{
				fprintf(stderr,"Can't execute command\n");
				fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [laucher.c]\n");
				exit(EXIT_FAILURE);	
			}
			
		} /*END PROGRAM*/




		//execution by command
		if(pr1->result==COMMAND)
		{
			strcpy(command,pr1->argv[0]);
			
			if((execvp(command,pr1->argv))<0)
			{
				fprintf(stderr,"Can't execute command\n");
				fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [laucher.c]\n");
				exit(EXIT_FAILURE);	
			}
		} /*END COMMAND*/
		
		
	}
	else /*parent*/
	{
		close ( pipein [1]);
		close ( pipein [0]);
		waitpid(process,0,0);
	}
	//END COMMAND 1
	
	
	memset(command,0, sizeof(char)*2048);
	
	
	if((process=fork())<0)
	{
		fprintf(stderr,"Error on process child creation, %s\n",strerror(errno));
		fprintf(stderr,"Error in ExecuteFGPipeProcess in [launcher.c]\n");
		return -1;
	}
	
	if(process==0) /*child*/
	{	
		
		close ( pipeout[1]);
		dup2 ( pipeout[0] , STDIN_FILENO );

		//execution by file
		if(pr2->result==PROGRAM)
		{
			//obtaining the full program path
			if(*(pr2->argv[0])=='/')
			{
				strcpy(command,pr2->argv[0]);
			}
			else
			{
				//get the absolute path of the cwd
				if(!getcwd(command,1024))
				{
					if(errno==ERANGE && !getcwd(command,2000))
					{
					//ERROR
						fprintf(stderr,"error in directory\n");
						fprintf(stderr,"Could not obtain cwd\n");
						fprintf(stderr,"buffer too short\n");
						fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [launcher.c]\n");
						return -1;
					}
					else if(errno==EACCES)
					{
						fprintf(stderr,"error in directory\n");
						fprintf(stderr,"Access Denied in cwd path\n");
						fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [laucher.c]\n");
						return -1;
					}
				}
				
				//directory now contains the cwd absolute path
				//Now we concat the cwd path to the relative path 
				//from cwd contained in [char* p]
				strcat(command,"/\0");
				strcat(command,pr2->argv[0]);
			}
	
			if((execv(command,pr2->argv))<0)
			{
				fprintf(stderr,"Can't execute command\n");
				fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [laucher.c]\n");
				exit(EXIT_FAILURE);	
			}
			
		} /*END PROGRAM*/




		//execution by command
		if(pr2->result==COMMAND)
		{
			strcpy(command,pr2->argv[0]);
			
			if((execvp(command,pr2->argv))<0)
			{
				fprintf(stderr,"Can't execute command\n");
				fprintf(stderr,"Implementation in ExecuteFGPipeProcess in [laucher.c]\n");
				exit(EXIT_FAILURE);	
			}
		} /*END COMMAND*/
		
		
	}
	else /*parent*/
	{
		close(pipeout[1]);
		waitpid(process,0,0);
		return 1;
	}
	//END COMMAND 2

	
}















