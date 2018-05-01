/*
 * EchoOnOff.cpp
 *
 *  Created on: Feb 18, 2016
 *      Author: dad
 */

#include <stdio.h>
#ifdef WINDOZE
//#include <../passlin/windows.h>
#include <windows.h>
#else
#include <termios.h>
#include <unistd.h>
#endif

int havemodeterminfo = 0;
#ifdef WINDOZE
unsigned long oldmodeterm = 0;
unsigned long newmodeterm;
extern HANDLE hStdin;
#else
termios oldmodeterm;
termios newmodeterm;
#endif

int EchoOff(){
	if ( havemodeterminfo == 0 ) {
#ifdef WINDOZE
		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hStdin, &oldmodeterm);
		newmodeterm = oldmodeterm  & (~ENABLE_ECHO_INPUT);
#else
		tcgetattr(STDIN_FILENO, &oldmodeterm);
		newmodeterm = oldmodeterm;
		newmodeterm.c_lflag &= ~ECHO;
#endif
		havemodeterminfo = 1;
	}

#ifdef WINDOZE
		return ( (int)SetConsoleMode(hStdin, newmodeterm) );
#else
		return ( tcsetattr(STDIN_FILENO, TCSANOW, &newmodeterm) );
#endif

}

int EchoOn(){
	if ( havemodeterminfo == 0 ) {
#ifdef WINDOZE
		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hStdin, &oldmodeterm);
		newmodeterm = oldmodeterm  & (~ENABLE_ECHO_INPUT);
#else
		tcgetattr(STDIN_FILENO, &oldmodeterm);
		newmodeterm = oldmodeterm;
		newmodeterm.c_lflag &= ~ECHO;
#endif
		havemodeterminfo = 1;
	}
    printf("\n");

#ifdef WINDOZE
		return ( (int)SetConsoleMode(hStdin, oldmodeterm) );
#else
		return ( tcsetattr(STDIN_FILENO, TCSANOW, &oldmodeterm) );
#endif

}

int EchoIsOn(){

#ifdef WINDOZE
	unsigned long mymodeterm = 0;
#else
	termios mymodeterm;
#endif

#ifdef WINDOZE
		hStdin = GetStdHandle(STD_INPUT_HANDLE);
		GetConsoleMode(hStdin, &mymodeterm);
		if ( mymodeterm & ENABLE_ECHO_INPUT ) return(1);
#else
		tcgetattr(STDIN_FILENO, &mymodeterm);
		if ( mymodeterm.c_lflag & ECHO ) return (1);
#endif

		return(0);
}



