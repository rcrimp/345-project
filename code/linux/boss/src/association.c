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

/* association
 * This file contains the functions to determine the association between 
 * extensions and programs.
 * The implementation open the config file and serarch into it for 
 * lines such as c|vi that means that when I write on the prompt
 * $ ,/main.c it opens main.c with vi.
 * The command can contains options like cp -r etc....
 */


#include "association.h"

//This function take the filename as parameter
//get his extension and find in the config file 
//the relative program (if exist).
//This function returns the string of the program if exists
//otherwise returns NULL
char* FindFileAssociation(char *line)
{
	FILE *stream=NULL;
	char conffile[2048];
	char ext[2048],ext2[2048];
	char *home=NULL;
	char *result=NULL;
	char *p=NULL;
	char *l=NULL,*j=NULL, *k=NULL;;
	char *temp=NULL;
	int n=0;
	int i=0;
	
	if(!line)
	{
		fprintf(stderr,"ERROR!!! line parameter is NULL\n");
		fprintf(stderr,"This error could not happen!!! what's wrong!!! HEEEEEEELP!!!\n");
		fprintf(stderr,"Error in FindFileAssociation in [association.c]\n");
		return NULL;
	}
	
	//allocating the string that contains the result
	result=malloc(2048);
	if(!result)
	{
		fprintf(stderr,"Could not allocate the memory for the result string\n");
		fprintf(stderr,"Error in FindFileAssociation in [association.c]\n");
		return NULL;
	}
	//Zeroing out the structure
	memset(result,0,2048);
	memset(ext,0,2048);
	memset(ext2,0,2048);
	
	//searching the extension .something
	//we simply search the first dot in the string line
	p=line;
	while(*p!='.')
	{
		if(p=='\0')
		{
			fprintf(stderr,"There is no extension in the filename!!!\n");
			fprintf(stderr,"Could not continue! Error in FindFileAssociation in [association.c]\n");
			free(result);
			result=NULL;
			return NULL;
		}
		p++;
	}
	p++; //step to the extensions after the dot
	
	i=0;
	while(*p!=' ' && *p!='\n' && *p!='\0')
	{
		ext2[i]=*p;
		p++;
		i++;	
	}
	
	//Opening the config file
	home=getenv("HOME");
	if(!home)
	{
		fprintf(stderr,"Could not take the home directory!\n");
		fprintf(stderr,"Error in FindFileAssociation [association.c]\n");
		return NULL;
	}
	memset(conffile,0,2048);
	strcpy(conffile,home);
	strcat(conffile,"/.boss_conf");
		
	stream=fopen(conffile,"r");
	
	if(!stream)
	{
		fprintf(stderr,"There is no config file [~/.boss_conf]\n");
		fprintf(stderr,"Could not continue! Error in FindFileAssociation in [association.c]\n");
		free(result);
		result=NULL;
		return NULL;
	}
	
	
	//finding the first tag @start boss config
	while(1)
	{
	
		if(getline(&temp,&n,stream)==-1)
		{
			fprintf(stderr,"Warning!reached the end of configuration file\n");
			fprintf(stderr,"whithout finding tags\n");
			fprintf(stderr,"No association file found\n");
			fclose(stream);
			free(result);
			result=NULL;
			return NULL;
		}
		l=temp;
		
		//space removal
		while(*l==' ')l++;
		
		if(*l=='@')
		{
		
		
			//verifying
			j=l;
			while(*j!='\0')
			{
				if(*j=='\n')
				{
					*j='\0';
					break;
				}
				j++;
			}
			
			if(strcmp(l,"@start association\0")!=0)
			{
				fprintf(stderr,"Warning!error in the parsing of configuration file\n");
				fprintf(stderr,"could not find @start association config\n");
				fprintf(stderr,"No association file found\n");
			}	
			else
			{
				break;
			}	
		}
	}
	
	
	
	
	//finding the program association
	while(1)
	{
		if(getline(&temp,&n,stream)==-1)
		{
			fprintf(stderr,"Warning!reached the end of configuration file\n");
			fprintf(stderr,"whithout finding tags\n");
			fprintf(stderr,"No association file found\n");
			fclose(stream);
			free(result);
			result=NULL;
			return NULL;
		}
		
		
		l=temp;
		
		if(strcmp(l,"@end association\0")==0)
		{
			fprintf(stderr,"Warning!reached the end of configuration tag\n");
			fprintf(stderr,"whithout finding associations\n");
			fprintf(stderr,"No association file found\n");
			fclose(stream);
			free(result);
			result=NULL;
			return NULL;
		}
		
		
		while(*l==' ') l++;
		
		//copyng the extension
		//memset(ext,0,2048);
		for(i=0; i<2047; i++)
		{
			//we exit if we reach the end of the config string
			//because we have no program associated with the extension
			if(*l=='\0' || *l=='\n')
			{
				fprintf(stderr,"Error! Bad config option\n");
				fprintf(stderr,"in the association settings\n");
				fprintf(stderr,"No program associated to an extension\n");
				fclose(stream);
				free(result);
				result=NULL;
				return NULL;
			}
			
			//we find the estension!
			//we can exit by this for to control if
			//the extension is the one we are searching for
			if(*l==' ' || *l=='|')
			{
				//space and '|' removal
				while(*l==' ' || *l=='|') 
				{
					l++;
				}
				break;
			}
			
			ext[i]=*l;
			l++;
		}
		ext[i+1]='\0';
		
		if(strcmp(ext2,ext)==0)
		{
		
			//return the program name with parameters
			//associated to the extension
			strcpy(result,l);
			fclose(stream);
			return result;
		}
		
	}//end while find the program association
	
}

