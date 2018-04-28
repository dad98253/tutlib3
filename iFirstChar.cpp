/*
 * iFirstChar.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>

// get the index of first non-blank character in the buf string
// if it's all blank, the index will point past the end of the string
// (i.e., equal to the length of the string)
int iFirstChar(char * buf){
	int iCount;
	int ilen = strlen(buf);
	for(iCount=0;iCount<ilen;iCount++) {
		if(buf[iCount] != ' ' ) break;
	}
	return(iCount);
}



