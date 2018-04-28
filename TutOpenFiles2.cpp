/*
 * TutOpenFiles2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

extern int MsgBox(const char * message, ...);

int TutOpenFiles2 ( int iFormat, FILE **fDBFile, FILE **fSeqFile, FILE **fDirectFile , FILE **fIndexFile){
	char * szOpenMode;
	char * szModeMsg;

	if ( iFormat != 1 && iFormat != 2 && iFormat != 3 && iFormat != 4) return (-1);
	if ( iFormat == 1 || iFormat == 4) {
		szOpenMode = (char *)"w+b";
		szModeMsg = (char *)"write";
	} else {
		szOpenMode = (char *)"rb";
		szModeMsg = (char *)"read";
	}

	if ((*fDBFile = fopen("fort.1", "r")) == NULL) {
		MsgBox("unable to open tutorial database file \"%s\" for read","fort.1");
		return(-2);
	}

	if ((*fSeqFile = fopen("tutSeq.61", szOpenMode)) == NULL) {
		MsgBox("unable to open tutorial name file \"%s\" for %s","tutSeq.61",szModeMsg);
		return(-3);
	}

	if ( iFormat >= 3 ) return(0);

	if ((*fDirectFile = fopen("tutRan.60", szOpenMode)) == NULL) {
		MsgBox("unable to open tutorial random file \"%s\" for %s","tutRan.60",szModeMsg);
		return(-4);
	}
	if ((*fIndexFile = fopen("tutIndex.60", szOpenMode)) == NULL) {
		MsgBox("unable to open tutorial index file \"%s\" for %s","tutIndex.60",szModeMsg);
		return(-5);
	}

	return(0);
}



