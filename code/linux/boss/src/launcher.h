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
 
 
#ifndef _LAUNCHER_
#define _LAUNCHER_

#include "shinclude.h"
#include "parsertypes.h"

//This function launch a single program in foreground
int ExecuteFGSingleProcess(struct parseresult* pr);

//This function launch two programs linked by a pipe
int ExecuteFGPipeProcess(struct parseresult* pr1, struct parseresult* pr2);

#endif
