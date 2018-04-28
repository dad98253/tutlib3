/*
 * tutSwitchToTTY.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#ifdef WINDOZE
#include <io.h>
#else
#include <unistd.h>
#endif
#include "tutlib.h"

extern int MsgBox(const char * message, ...);

int tutSwitchToTTY( TUTSTATUSDATA *lpTStatus)
{
	if ( lpTStatus->itutInputSource == 0 ) {
		MsgBox("Switching to keyboard input...");
		fclose(lpTStatus->fTutInputStream);
		fclose(lpTStatus->fTutOutputStream);
		if( (lpTStatus->fTutInputStream = fopen("/dev/tty","r")) == NULL ) {
			MsgBox("Unable to open keyboard for input...");
			return(-98);
		}
		if ( !isatty(fileno(stdout)) ) {
			if( (lpTStatus->fTutOutputStream = fopen("/dev/tty","a")) == NULL ) {
				MsgBox("Unable to open terminal/console for output...");
				return(-98);
			}
		}
		lpTStatus->itutInputSource = 1;
		return(-1);		// return -1 to force re-prompt to console
	}
	return(lpTStatus->itutInputSource);
}



