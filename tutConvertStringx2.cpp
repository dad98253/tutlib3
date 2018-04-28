/*
 * tutConvertStringx2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"
#include "tuterrors.h"

int tutConvertString (char * szString, int iStringSize) {
	if ( szString == NULL ) {
#ifdef DEBUG
		if (debugflag) dfprintf(fp9,"szString == NULL in tutConvertStringx2 at line %i\n",__LINE__);
#endif
		return(BADUCVRTCCALL);
	}
	if ( iStringSize <= 0 ) {
#ifdef DEBUG
		if (debugflag) dfprintf(fp9,"iStringSize <= 0 in tutConvertStringx2 at line %i\n",__LINE__);
#endif
		return(BADUCVRTCCALL);
	}
	if ( sztutNextLine == NULL ) {
#ifdef DEBUG
		if (debugflag) dfprintf(fp9,"sztutNextLine == NULL in tutConvertStringx2 at line %i\n",__LINE__);
#endif
		return(BADUCVRTCCALL);
	}

	strncpy(szString,(const char *)sztutNextLine,iStringSize);
	szString[iStringSize-1] = '\000';

	return(SUCCESS);
}



