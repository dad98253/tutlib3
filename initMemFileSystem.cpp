/*
 * initMemFileSystem.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"


// initialization of memfile system (type 2)
// you only to call this when building a new memfile system
int initMemFileSystem(int iMemFileSize, int iMemDirSize) {

#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"calling initMemFileSystem: iMemFileSize,iMemDirSize = %i, %i\n",iMemFileSize,iMemDirSize);
#endif

//	struct DIRREC2 {
//		long unsigned int FileOffset;
//		long unsigned int NameOffset;
//	    short unsigned int wFileSize;
//	    short unsigned int wUncompressedFileSize;
//	    short unsigned int wNamelength;
//	    Byte	flags;
//	 };
//	#define BUFFERSIZE	50000
//  #define DIRSIZE		10

	if ( iMemFileSize > BUFFERSIZE ) uncomprMemFileSystemLen = (uLong)iMemFileSize;
	if ( iMemDirSize > DIRSIZE ) uMaxDirSize = (uLong)iMemDirSize;

	if ( (Directory  = (DIRREC2*)calloc((uInt)iMemDirSize, sizeof(DIRREC2))) == NULL) return(0);

	if ( (FileNameList  = (char **)calloc((uInt)iMemDirSize, sizeof(char *))) == NULL) return(0);

	if ( (uncomprMemFileSystem = (Byte *)calloc((uInt)uncomprMemFileSystemLen, 1)) == NULL) return(0);

	iFileNumber = 0;
	TotalMemFileSystemSize = sizeof(long unsigned int);   // save space for offset to file directory
	short unsigned int suTemp = (short unsigned int)uMaxDirSize;
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,&suTemp,sizeof(short unsigned int));   //  save directory size
	TotalMemFileSystemSize += sizeof(short unsigned int);

#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"initMemFileSystem created Directory of length %i, initial TotalMemFileSystemSize = %u\n",uMaxDirSize,TotalMemFileSystemSize);
#endif

	return(uMaxDirSize);
}



