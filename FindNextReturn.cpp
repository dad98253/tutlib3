/*
 * FindNextReturn.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>

int FindNextReturn( char * srReturnTerminatedString ) {
	return((int)strcspn ( (const char * )srReturnTerminatedString, "\n" ));
}



