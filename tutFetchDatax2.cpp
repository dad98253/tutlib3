/*
 * tutFetchDatax2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

float tutFetchDatax2 (int iIndex) {

	if (iIndex < 1 ) return ((float)-9999.9);
	int iIntValue;
	if(ParseLineRCItems == NULL)return ((float)-9997.0);
	if(ParseLineRCItems[iIndex-1] == NULL)return ((float)-9998.0);
	sscanf(ParseLineRCItems[iIndex-1],"%i",&iIntValue);

	return((float)iIntValue);
}



