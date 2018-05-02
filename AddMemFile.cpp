/*
 * AddMemFile.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "zlib.h"
#include "tutlib.h"

// add the iFileSize bytes pointed to by lpMemfile to the memory file system
// assign a file name of szFileName to it
// if iOption == 1, then compress the data before saving it.
int AddMemFile(Byte * lpMemfile, int iFileSize, char * szFileName, int iOption) {

#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"in AddMemFile, szFileName = %s, iFileSize = %i, iOption = %i\n",szFileName,iFileSize,iOption);
#endif

	int err;

	if ( iOption != 0 && iOption != 1 ) return(-3);
//	struct DIRREC {
//		long unsigned int FileOffset;
//		long unsigned int NameOffset;
//	    short unsigned int wFileSize;
//	    short unsigned int wNamelength;
//	 };
//	#define DIRSIZE		1
	if ( (uLong)iFileNumber == uMaxDirSize ) return(-2);
	int i;
	for (i=0;i<iFileNumber;i++) {
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"in AddMemFile, FileNameList[%i] = \"%s\", szFileName = \"%s\"\n",i,FileNameList[i],szFileName);
#endif
		if ( strcmp(FileNameList[i],szFileName) == 0 ) return (-4);
	}
	char * szTempName;
	if ( (szTempName  = (char *)calloc((uInt)(strlen(szFileName)+1), sizeof(char))) == NULL) return(-1);
	strcpy(szTempName,szFileName);

	FileNameList[iFileNumber] = szTempName;		// save the file name in the temprary filelist

#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"in AddMemFile, offset for %s = %u bytes\n",szFileName,TotalMemFileSystemSize);
#endif

	// start of file data... set directory data

	(Directory+iFileNumber)->FileOffset = TotalMemFileSystemSize;
	(Directory+iFileNumber)->wNamelength = (short unsigned int)strlen(FileNameList[iFileNumber]);
	(Directory+iFileNumber)->flags = (Byte)iOption;
	(Directory+iFileNumber)->wFileSize = iFileSize;
	(Directory+iFileNumber)->wUncompressedFileSize = iFileSize;
	//	struct DIRREC2 {
	//		long unsigned int FileOffset;
	//		long unsigned int NameOffset;
	//	    short unsigned int wFileSize;
	//	    short unsigned int wUncompressedFileSize;
	//	    short unsigned int wNamelength;
	//	    Byte flags;
	//	 };
	//	 char *FileNameList[uMaxDirSize];

	if ( iOption == 1 ) {
		uLong comprLen = iFileSize + 1000;  // add a little extra just in case the array doesn't shrink properly (possible for very small and/or uncompressable files)
		Byte *compr    = (Byte*)calloc((uInt)comprLen, 1);

		err = compress(compr, &comprLen, lpMemfile, iFileSize);  // note that comprLen is both input and output. It checks to make sure not to over index the array...
		CHECK_ERR(err, "compress");

		(Directory+iFileNumber)->wFileSize = (unsigned short)comprLen;

		if ( (TotalMemFileSystemSize+comprLen) > uncomprMemFileSystemLen ) {
#ifdef DEBUG
			if (debugflag ) dfprintf(fp9,"memFile is full... compressed file size = %u, TotalMemFileSystemSize = %u, uncomprMemFileSystemLen = %u\n",comprLen,TotalMemFileSystemSize,uncomprMemFileSystemLen);
#endif
			return(-20);
		}
		memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,compr,comprLen);   //  save the file
		TotalMemFileSystemSize += comprLen;
		free(compr);
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"compressed file size = %u, file saved as directory entry number %i\n",comprLen,iFileNumber);
#endif
	} else {
		if ( (TotalMemFileSystemSize+iFileSize) > uncomprMemFileSystemLen ) {
#ifdef DEBUG
			if (debugflag ) dfprintf(fp9,"memFile is full... file size = %i, TotalMemFileSystemSize = %u, uncomprMemFileSystemLen = %u\n",iFileSize,TotalMemFileSystemSize,uncomprMemFileSystemLen);
#endif
			return(-20);
		}
		memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,lpMemfile,iFileSize);   //  save the file
		TotalMemFileSystemSize += iFileSize;
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"file saved as directory entry number %i\n",iFileNumber);
#endif
	}
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"TotalMemFileSystemSize is now %u\n",TotalMemFileSystemSize);
#endif
		iFileNumber++;
		short unsigned int DSize;
		DSize = (short unsigned int)iFileNumber;
		memcpy(uncomprMemFileSystem+sizeof(long unsigned int),&DSize,sizeof(short unsigned int));
		return(iFileNumber-1);
	}



