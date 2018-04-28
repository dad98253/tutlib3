/*
 * tutNumCharInInputBuffer.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

int tutNumCharInInputBuffer(TUTSTATUSDATA *lpTStatus){

	return(lpTStatus->iLastCharInBuffer-lpTStatus->iNextCharInBuffer);

}



