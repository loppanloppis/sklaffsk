/* color.c */

/*
 *   SklaffKOM, a simple conference system for UNIX.
 *
 *   Copyright (C) 1993-1994  Torbj|rn B}}th, Peter Forsberg, Peter Lindberg, 
 *                            Odd Petersson, Carl Sundbom
 *
 *   Program dedicated to the memory of Staffan Bergstr|m.
 *
 *   For questions about this program, mail sklaff@skom.se
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2, or (at your option)
 *   any later version.
 *    
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *   
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the Free Software
 *   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include "sklaff.h"
#include "ext_globals.h"

/*
 * color_high - sends ansi color strings, highlighted
 * args: value, colorvalue
 * ret: 
 */

void color_high(value)
int value;
{
  LINE colstr;

  if (UseAnsi) {
    sprintf(colstr,"\033[1;%dm", value);
    fputs(colstr, stdout);
    strcpy(last_color,colstr);
  }
}

/*
 * color_string - sends ansi color strings, highlighted
 * args: value, colorvalue
 * ret: 
 */

char *color_string(value, colstr)
int value;
char *colstr;
{

  if (UseAnsi) {
    sprintf(colstr,"\33[1;%dm", value);
    return colstr;
  }
  return "";
}

/*
 * color_low - sends ansi color strings, highlighted
 * args: value, colorvalue
 * ret: 
 */

void color_low(value)
int value;
{
  LINE colstr;

  if (UseAnsi) {
    sprintf(colstr,"\033[0;%dm", value);
    fputs(colstr, stdout);
    strcpy(last_color,colstr);
  }
}

/*
 * color_restore - sends ansi color strings, highlighted
 * args: value, colorvalue
 * ret: 
 */

void color_restore(value)
LINE value;
{
  if (UseAnsi) {
    fputs(value, stdout);
  }
}

