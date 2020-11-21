/*
 * tutIsTty.cpp
 *
 *  Created on: Jun 2, 2018
 *      Author: dad
 */

#ifndef WINDOZE
#include <unistd.h>
#endif	// WINDOZE

#include "debug.h"


bool bOutputDP	=false;
bool bIsStdinTty 	=false;

#ifdef WINDOZE
#ifndef TEXTMODEONLY
extern bool bStdioRedirectedToConsole;
#endif  // TEXTMODEONLY
#endif  // WINDOZE


int tutIsTty() {

	// check if stdin is tty...
#ifdef WINDOZE
#ifndef TEXTMODEONLY
	if ( bStdioRedirectedToConsole ) { bOutputDP = bIsStdinTty = true; } else { bOutputDP = bIsStdinTty = false; }
#else  //  TEXTMODEONLY
	if ( isatty(STDIN_FILENO) != 0 ) { bOutputDP = bIsStdinTty = true; } else { bOutputDP = bIsStdinTty = false; }
#endif  //  TEXTMODEONLY
#else  //  WINDOZE
    if ( isatty(STDIN_FILENO) != 0 ) {
//	if ( isatty(fileno(stdin)) != 0 ) {
		bOutputDP = bIsStdinTty = true; }
	else {
		bOutputDP = bIsStdinTty = false; }
#endif  //  WINDOZE
#ifdef DEBUG
		if (bIsStdinTty) {
			dfprintf2(__LINE__,__FILE__,TRACE,"stdin is a tty\n");
		} else {
			dfprintf2(__LINE__,__FILE__,TRACE,"stdin is not a tty\n");
		}
#endif  // DEBUG

	return(bOutputDP);
}

