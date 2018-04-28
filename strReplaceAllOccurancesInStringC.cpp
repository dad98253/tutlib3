/*
 * strReplaceAllOccurancesInStringC.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>

int strReplaceAllOccurancesInStringC(const char * szString,const char * szOldChar,const char * szNewChar,char * szResult){
	int iNumberFound=0;
	int i;
	szResult[0] = '\000';
	int iEnd = strlen(szString);
	if (iEnd == 0) return(0);
	if (strlen(szOldChar) == 0) return(-1);
	bool bDeleteChar = false;
	if (strlen(szNewChar) == 0) bDeleteChar = true;
	for (i=0;i<iEnd;i++) {
		if ( szString[i] == *szOldChar ) {
			if (!bDeleteChar) {
				*szResult = *szNewChar;
				szResult++;
			}
			iNumberFound++;
		} else {
			*szResult = szString[i];
			szResult++;
		}
	}
	*szResult = '\000';
	return(iNumberFound);
}



