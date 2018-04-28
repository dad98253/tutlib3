/*
 * TutCloseFiles2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

extern int MsgBox(const char * message, ...);

int TutCloseFiles2 ( int iFormat, FILE *fDBFile, FILE *fSeqFile, FILE *fDirectFile , FILE *fIndexFile){

	if ( iFormat != 1 && iFormat != 2 && iFormat != 3 && iFormat != 4 ) return (-1);

	if ( fclose(fDBFile) == EOF) {
		MsgBox("unable to close tutorial database file");
		return(-2);
	}
	if ( fclose(fSeqFile) == EOF) {
		MsgBox("unable to close tutorial name file");
		return(-3);
	}
	if ( iFormat >= 3 ) return(0);
	if ( fclose(fDirectFile) == EOF) {
		MsgBox("unable to close tutorial random file");
		return(-4);
	}
	if ( fclose(fIndexFile) == EOF) {
		MsgBox("unable to close tutorial index file");
		return(-4);
	}

	return(0);
}



