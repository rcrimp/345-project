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

/* parser types
 * Contains the types used by the parser and the launcher
 */

#ifndef _PARSERTYPES_
#define _PARSERTYPES_

#include "shinclude.h"

//arguments length
#define ARGLEN 256

enum PARSRESTYPE
{
	PROGRAM=0,
 	COMMAND
};

enum REDIRECTION
{
	NONE=0,
	STDIN,
	STDOUT,
	STDERR
};

struct parseresult
{
	enum PARSRESTYPE result; //type of command parsed
	int argc; //number of arguments
	char **argv; //arguments list
	
	enum REDIRECTION redirtype; //the type of redirection
	int isappend; //if the redirection is in append mode
	char redfilename[256]; //filename for redirection
};

#endif
  
