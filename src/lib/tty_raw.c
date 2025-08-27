/* tty_raw.c */

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
 * tty_raw - sets terminal in raw mode
 */

void tty_raw()
{

#ifdef BSDTERMIO
    struct sgttyb temp_mode;
    struct winsize window;
    
    ioctl(0, TIOCGETP, (char *) &temp_mode);
    memcpy(&Tty_mode, &temp_mode, sizeof(struct sgttyb));
    temp_mode.sg_flags |= RAW;
    temp_mode.sg_flags &= ~ECHO;
    ioctl(0, TIOCSETP, (char *) &temp_mode);
    ioctl(0, TIOCGWINSZ, &window);
    Numlines = window.ws_row;
    if (!Numlines) Numlines = 24;
#else 
    struct termio temp_mode;
    
    ioctl(0, TCGETA, (char *) &temp_mode);
    memcpy(&Tty_mode, &temp_mode, sizeof(struct termio));
    temp_mode.c_iflag = 0;
    temp_mode.c_oflag &= ~OPOST;
    temp_mode.c_lflag &= ~(ISIG | ICANON | ECHO | XCASE);
    temp_mode.c_cc[VMIN] = 1;
    temp_mode.c_cc[VTIME] = 0;
    ioctl(0, TCSETA, (char *) &temp_mode);
    Numlines = 24;
#endif
    Lines = 1;
}
