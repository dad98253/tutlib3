/*
 * tutGetNextLineFromBuffer.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"

extern int MsgBox(const char * message, ...);

int tutGetNextLineFromBuffer( TUTSTATUSDATA *lpTStatus, char ** sztutNextLineIn) {
	int iSpan;
	if (*sztutNextLineIn != NULL) {
		free(*sztutNextLineIn);
		*sztutNextLineIn = NULL;
	}
	iSpan = strcspn (lpTStatus->szTypeAheadBuffer+lpTStatus->iNextCharInBuffer,";");
	if ( (*sztutNextLineIn = (char *)calloc(1,iSpan+1)) == NULL ) {
		MsgBox("memory allocation failed -- out of memory");
		return(-99);
	}
	strncpy(*sztutNextLineIn,lpTStatus->szTypeAheadBuffer+lpTStatus->iNextCharInBuffer,iSpan);
	*(*sztutNextLineIn+iSpan) = '\000';
	lpTStatus->iNextCharInBuffer += (iSpan+1);
	if ( lpTStatus->iNextCharInBuffer > lpTStatus->iLastCharInBuffer ) lpTStatus->iNextCharInBuffer = lpTStatus->iLastCharInBuffer;
	lpTStatus->iLogicalLineNumber++;
	return(strlen(*sztutNextLineIn));
}



