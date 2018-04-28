/*
 * strFindLastNonOccuranceC.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>

int strFindLastNonOccuranceC ( const char *  buf, const char * szTestStr ) {
	int iBufLen = strlen(buf);
	if (iBufLen < 1 ) return (0);
	if (strlen(szTestStr) < 1 ) return(iBufLen);
	int i;
	int iLast = 0;
	for(i=0;i<iBufLen;i++) {
		if (buf[i] != *szTestStr ) iLast = i;
	}

	return(iLast);
}



