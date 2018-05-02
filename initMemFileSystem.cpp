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
	// we have a problem using long int on different platforms/compilers (e.g., vc6 vs. gcc)
	// therefore, we will set the first four bytes to some info that will assure that the MemSource.h
	// file is compatible with the current system
	// The format presently will only work with MemSource.h's generated on computers of compatible endian and
	// assumes that we do not need to know the length of a long long
	// first byte:		The first byte is used to identify the MemSource.h format/version presently it is set to 0x95
	//					This byte could be used to specify endianness in the future. If so, 0x95 should be little endian
	// second byte:		The size (in bytes) of short int
	// third byte:		The size (in bytes) of int
	// fourth byte:		The size (in bytes) of long int
	TotalMemFileSystemSize = 0;
	unsigned char bMemFileFormat = 0x95;
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,&bMemFileFormat,1);		//  save memfile format
	TotalMemFileSystemSize++;
	unsigned char bShortIntSize = sizeof(short unsigned int);
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,&bShortIntSize,1);		//  The size of short int
	TotalMemFileSystemSize++;
	unsigned char bIntSize = sizeof(unsigned int);
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,&bIntSize,1);			//  The size of int
	TotalMemFileSystemSize++;
	unsigned char bLongIntSize = sizeof(long unsigned int);
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,&bLongIntSize,1);		//  The size of long int
	TotalMemFileSystemSize++;
	TotalMemFileSystemSize += sizeof(long unsigned int);   						// save space for offset to file directory (filled in later)
	short unsigned int suTemp = (short unsigned int)uMaxDirSize;
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,&suTemp,sizeof(short unsigned int));   //  save directory size (corrected in AddMemFile)
	TotalMemFileSystemSize += sizeof(short unsigned int);

#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"initMemFileSystem created Directory of length %i, initial TotalMemFileSystemSize = %u\n",uMaxDirSize,TotalMemFileSystemSize);
#endif

	return(uMaxDirSize);
}



