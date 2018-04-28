/*
 * tutPrompt.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"

extern int MsgBox(const char * message, ...);

int tutPrompt( TUTSTATUSDATA *lpTStatus, char * szDPIDString,char *szDefaultInput,char *szDPText) {

	int iNumBytesOutput = 0;

// if its a tty and the input buffer is empty, print a prompt
	bOutputDP = tutNumCharInInputBuffer(lpTStatus) == 0 && bIsStdinTty;
	// if stdin is a tty and the type-ahead buffer is empty, display the display point ID - note no CR, first line of DP finishes the line
	if( strlen(szDPIDString) != 0 && bOutputDP) {
		iNumBytesOutput = fprintf(stdout," %s",szDPIDString);
		if ( iNumBytesOutput < 0 ) {
			MsgBox ("print of DPID failed at line %i in tutPrompt\n",__LINE__);
			return (iNumBytesOutput);
		}
	}
#ifdef DEBUG
		if ( debugflag && strlen(szDPIDString) != 0 && bOutputDP ) dfprintf(fp9," %s",szDPIDString);
#endif
// if stdin is a tty and the type-ahead buffer is empty, display the display point ID - note no CR, first line of DP finishes the line
	if( strlen(szDPText) != 0 && bOutputDP) {
		iNumBytesOutput = fprintf(stdout,"%s",szDPText);
		if ( iNumBytesOutput < 0 ) {
			MsgBox ("print of DP text failed at line %i in tutPrompt\n",__LINE__);
			return (iNumBytesOutput);
		}
	}
#ifdef DEBUG
		if ( debugflag && strlen(szDPText) != 0 && bOutputDP ) dfprintf(fp9,"%s",szDPText);
#endif

// if stdin is a tty and the type-ahead buffer is empty, print a prompt
	if( bOutputDP ) {
		if ( szDefaultInput == NULL ) {
			iNumBytesOutput = fprintf(stdout," ::");
			if ( iNumBytesOutput < 0 ) {
				MsgBox ("print of tutor prompt failed at line %i in tutPrompt\n",__LINE__);
				return (iNumBytesOutput);
			}
#ifdef DEBUG
			if (debugflag) dfprintf(fp9," ::\n");
#endif
		} else {
			if ( szDefaultInput[0] == '\000' ) {
				iNumBytesOutput = fprintf(stdout," ::");
				if ( iNumBytesOutput < 0 ) {
					MsgBox ("print of tutor prompt failed at line %i in tutPrompt\n",__LINE__);
					return (iNumBytesOutput);
				}
#ifdef DEBUG
				if (debugflag) dfprintf(fp9," ::\n");
#endif
			} else {
				iNumBytesOutput = fprintf(stdout," (%s)::\n",szDefaultInput);
				if ( iNumBytesOutput < 0 ) {
					MsgBox ("print of tutor prompt failed at line %i in tutPrompt\n",__LINE__);
					return (iNumBytesOutput);
				}
#ifdef DEBUG
				if (debugflag) dfprintf(fp9," (%s)::\n",szDefaultInput);
#endif
			}
		}
	}

	return(0);
}



