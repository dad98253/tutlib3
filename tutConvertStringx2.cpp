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
		dfprintf(__LINE__,__FILE__,TRACE,"szString == NULL in tutConvertStringx2 at line %i\n",__LINE__);
#endif
		return(BADUCVRTCCALL);
	}
	if ( iStringSize <= 0 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"iStringSize <= 0 in tutConvertStringx2 at line %i\n",__LINE__);
#endif
		return(BADUCVRTCCALL);
	}
	if ( sztutNextLine == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"sztutNextLine == NULL in tutConvertStringx2 at line %i\n",__LINE__);
#endif
		return(BADUCVRTCCALL);
	}

	strncpy(szString,(const char *)sztutNextLine,iStringSize);
	szString[iStringSize-1] = '\000';

	return(SUCCESS);
}



