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

#ifndef _ASSOCIATION_
#define _ASSOCIATION_

#include "shinclude.h"

//This function take the filename as parameter
//get his extension and find in the config file 
//the relative program (if exist).
//This function returns the string of the program if exists
//otherwise returns NULL
char* FindFileAssociation(char *line);

#endif

