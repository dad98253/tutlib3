/*
 * StripSingleQuotes.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"


int StripSingleQuotes(char * szGID){
	char * szTempString;
	int iLenNewString = strlen(szGID);
	if ( (szTempString = (char *)malloc(iLenNewString+1) ) == NULL ) {
		printf("malloc failed in StripSingleQuotes,  iLenNewString = %i\n",iLenNewString);
		return(-1);
	}
	strcpy(szTempString,szGID);
	strReplaceAllOccurancesInStringC((const char *)szTempString,"'","",szGID);
	free(szTempString);
	return(strlen(szGID));
}



