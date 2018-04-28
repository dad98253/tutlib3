/*
 * strCopySubstringC.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>

int strCopySubstringC(const char * szString,int iSubstringStart,int iSubstringEnd,char * szResult){
	szResult[0] = '\000';
	if (iSubstringStart >= iSubstringEnd) return (iSubstringEnd-iSubstringStart);
	if (strlen(szString) == 0) return(0);
	if (iSubstringStart > (int)strlen(szString)) return(-9999);
	if (iSubstringEnd > (int)strlen(szString)) return(-9999);
	int i;
	int j=0;
	for (i=iSubstringStart;i<=iSubstringEnd;i++,j++) {
		szResult[j] = szString[i];
	}
	szResult[j] = '\000';
	return(j);
}



