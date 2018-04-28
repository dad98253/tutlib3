/*
 * tutFlushTypeaheadBuffer.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

void tutFlushTypeaheadBuffer(TUTSTATUSDATA *lpTStatus)
{
	char ** szTemp;
	char * szTempp;
	szTempp=NULL;
	szTemp = &szTempp;
	while ( tutNumCharInInputBuffer(lpTStatus) > 0 ) {
		tutGetNextLineFromBuffer( lpTStatus, szTemp);
	}
	if (*szTemp != NULL) {
		free(*szTemp);
		*szTemp = NULL;
	}
	return;
}



