/* output.c */

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
#include <varargs.h>

/* 
 * output - outputs string
 * args: same as for printf
 * ret:	success (0), stop output (-1)
 */


int output(va_alist)
va_dcl
{
    va_list args2;
    unsigned char *fmt, *tmp, *tmp2, c, *p1, *p2, *p3;
    unsigned char fmt2[HUGE_LINE_LEN], outline[HUGE_LINE_LEN];
    unsigned char tmpline[LONG_LINE_LEN];
    LINE coltmp;
    
    va_start(args2);
    fmt = va_arg(args2, unsigned char *);
    vsprintf(fmt2, fmt, args2);
    va_end(args2);
    tmp = fmt2;
    tmp2 = outline;
    while (*tmp) {
	if (Beep || Special || (*tmp != 7)) {
	    *tmp2 = *tmp;
	    tmp++;
	    if (*tmp2 == '\n') {
		*tmp2 = '\r';
		tmp2++;
		*tmp2 = '\n';
	    }
	    if (Ibm) {
		if (*tmp2 == '}') *tmp2 = 134;
		else if (*tmp2 == '{') *tmp2 = 132;
		else if (*tmp2 == '|') *tmp2 = 148;
		else if (*tmp2 == ']') *tmp2 = 143;
		else if (*tmp2 == '[') *tmp2 = 142;
		else if (*tmp2 == 0x5c) *tmp2 = 153;
	    }
	    else if (Iso8859) {
		if (*tmp2 == '}') *tmp2 = 229;
		else if (*tmp2 == '{') *tmp2 = 228;
		else if (*tmp2 == '|') *tmp2 = 246;
		else if (*tmp2 == ']') *tmp2 = 197;
		else if (*tmp2 == '[') *tmp2 = 196;
		else if (*tmp2 == 0x5c) *tmp2 = 214;
	    }
	    else if (Mac) {
		if (*tmp2 == '}') *tmp2 = 140;
		else if (*tmp2 == '{') *tmp2 = 138;
		else if (*tmp2 == '|') *tmp2 = 154;
		else if (*tmp2 == ']') *tmp2 = 129;
		else if (*tmp2 == '[') *tmp2 = 128;
		else if (*tmp2 == 0x5c) *tmp2 = 133;
/*          }
            else if (Next) {
                if (*tmp2 == '}') *tmp2 = 0332;
                else if (*tmp2 == '{') *tmp2 = 0331;
                else if (*tmp2 == '|') *tmp2 = 0360;
                else if (*tmp2 == ']') *tmp2 = 0206;
                else if (*tmp2 == '[') *tmp2 = 0205;
                else if (*tmp2 == 0x5c) *tmp2 = 0226;
            */
	    }
	    tmp2++;
	}
	else {
	    tmp++;
	}
    }
    
    *tmp2 = '\0';
    
    p2 = outline;
    while (*p2 != '\0') {
	p1 = tmpline;
	while ((*p2 != '\n') && (*p2 != '\0')) {
	    *p1 = *p2;
	    if (Rot13 && (*p1 >= 'A') && (*p1 <= 'z')) {
		if ((*p1 >= 'A') && (*p1 <= 'Z')) {
		    *p1 = *p1 - 13;  /* Rot13 */
		    if (*p1 < 'A') *p1 = 'Z' - ('A' - *p1 - 1);
		}
		else if ((*p1 >= 'a') && (*p1 <= 'z')) {
		    *p1 = *p1 - 13;  /* Rot13 */
		    if (*p1 < 'a') *p1 = 'z' - ('a' - *p1 - 1);
		}
	    }
	    /* :DK:TODO: Is this correct code (from sklommon-1_30) */
            if (*p1 == '\002') {
              p2++;
              p3 = (unsigned char*)color_string(*p2, coltmp);
              while (*p3) {
                *p1 = *p3;
                p1++;
                p3++;
              }
              p2++;
            }
            else {
	      p1++;
	      p2++;
            }
	    /* :DK:TODO: Is this correct code (from sklommon-1_30) */
	}
	if (*p2 == '\n') {
	    *p1 = '\n';
	    p1++;
	    p2++;
	    if (!Cont) Lines++;
	}
	*p1 = '\0';
	if ((Lines >= Numlines) && Numlines && !Cont) {
	    printf(MSG_MORE); 
	    do {
		if (Timeout) {
		    alarm(60 * Timeout);
		}
		do c = getc(stdin);
		while (c == 255);
		/*		if (Strip) c &= 0x7f; Obsolete, 18/2 2000, OR */
		alarm(0);
		Warning = 0;
	    } while ((c != 'q') && (c != 'Q') && (c !=' ') && (c != '\r')
		     && (c != '\n') && (c != 3) && (c != 'c') && (c != 'C'));
	    printf("\r       \r"); 
	    make_activity_note();
	    Lines = 1;
	    if ((c == 'c') || (c == 'C')) Cont = 1;
	    if ((c == 'q') || (c == 'Q') || (c == 3)
		|| ((c == ' ') && (!Space || Special))) {  /*Special->Space=0 */
		if ((strlen(tmpline) == 2) &&
		    (tmpline[0] == '\r') &&
		    (tmpline[1] == '\n')) {
		    printf("%s", tmpline);
		}
		return -1;
	    }
	}
	fputs(tmpline, stdout);
    }
    return 0;
}


/* Output for external applications */

int outputex(va_alist)
va_dcl
{
    va_list args2;
    unsigned char *fmt, *tmp, *tmp2, c, *p1, *p2;
    unsigned char fmt2[HUGE_LINE_LEN], outline[HUGE_LINE_LEN];
    unsigned char tmpline[LONG_LINE_LEN];
    
    va_start(args2);
    fmt = va_arg(args2, unsigned char *);
    /*    vsprintf(fmt2, fmt, args2); */
    vprintf(fmt, args2);
    va_end(args2);
    return 0;

    /* No use to add Iso8859 support. Eight bit is stripped anyway
       when sent over network. */
}

