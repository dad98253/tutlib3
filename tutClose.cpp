/*
 * tutClose.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

int tutClose() {
	if ( sztutNextLine != NULL ) {
		free(sztutNextLine);
		sztutNextLine = NULL;
	}

	free(lpTutMemFile);		// free the tutor file
	lpTutMemFile = NULL;

	return(0);
}



