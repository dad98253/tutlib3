/*
 * UnloadMemDir.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

int UnloadMemDir(Byte ** lpMemDir) {
	if (!bMemDirLoaded) return(1);
	if(*lpMemDir != NULL ) free(*lpMemDir);
	bMemDirLoaded = false;
return(0);
}


