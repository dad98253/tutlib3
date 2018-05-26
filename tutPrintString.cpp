/*
 * tutPrintString.cpp
 *
 *  Created on: Jun 11, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"
#include "tuterrors.h"

extern int MsgBox(const char * message, ...);

int tutPrintString (const char * szText){

	if (szText == NULL) return(BADUPRSTRCCALL);

	if( strlen(szText) != 0 && bIsStdinTty) {
		int iNumBytesOutput = fprintf(stdout," %s\n",szText);
		if ( iNumBytesOutput < 0 ) {
			MsgBox ("print of string failed at line %i in tutPrintString",__LINE__);
			return (iNumBytesOutput);
		}
	}
#ifdef DEBUG
		if ( strlen(szText) == 0 || !bOutputDP ) dfprintf(__LINE__,__FILE__,TRACE,"tutPrintString called with bOutputDP = %u and string = \"%s\" (length %i)\n",bOutputDP,szText,strlen(szText));
		if ( strlen(szText) != 0 &&  bOutputDP ) dfprintf(__LINE__,__FILE__,TRACE," %s\n",szText);
#endif

	return(SUCCESS);
}

