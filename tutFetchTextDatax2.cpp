/*
 * tutFetchTextDatax2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

char * tutFetchTextData (int iIndex) {

	if (iIndex < 1 ) return (NULL);
	if(ParseLineRCItems == NULL)return (NULL);
	if(ParseLineRCItems[iIndex-1] == NULL)return (NULL);

	return(ParseLineRCItems[iIndex-1]);
}



