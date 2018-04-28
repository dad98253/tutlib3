/*
 * tutFetchDataTypex2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

int tutFetchDataType (int iIndex) {

	if (iIndex < 1 ) return (-9999);
	if (iIndex > ParseLineRCNumItems ) return (-9998);

	if(ParseLineRCfmtItem == NULL)return (-9997);

	return(ParseLineRCfmtItem[iIndex-1]);
}



