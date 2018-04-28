/*
 * FixGIDName.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"


int FixGIDName(char * buf, char * szGID, int iMaxGID) {
	int iLenBufString = strlen(buf);
	if(iLenBufString < 1 || iLenBufString > iMaxGID ) {
		printf("bad GID string detected by FixGIDName,  iLenBufString = %i\n",iLenBufString);
		return (-1);
	}
	int iFirstNonBlank = strspn(buf, " ");
	int iLastNonBlank  = strFindLastNonOccuranceC(buf, " ");
	szGID[0]='\000';
	int iLenNewString = strCopySubstringC(buf,iFirstNonBlank,iLastNonBlank,szGID);
	if (iLenNewString < 1 ) return(0);
	char * szTempString;
	if ( (szTempString = (char *)malloc(iLenNewString+1) ) == NULL ) {
		printf("malloc failed in FixGIDName,  iLenNewString = %i\n",iLenNewString);
		return(-1);
	}
	strcpy(szTempString,szGID);
	strReplaceAllOccurancesInStringC((const char *)szTempString," ","_",szGID);
	strcpy(szTempString,szGID);
	strReplaceAllOccurancesInStringC((const char *)szTempString,"-","_",szGID);
	free(szTempString);
	return (0);
}



