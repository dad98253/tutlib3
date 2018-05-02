/*
 * CloseMemFileSystem.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"

int CloseMemFileSystem() {

	DIRREC2 * DirEntry;

#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"calling CloseMemFileSystem\n");
#endif
	int ijck;
	int iSizeOfAllNames = 0;
	if ( iFileNumber < 1 ) return (0);
	for (ijck=0;ijck<iFileNumber;ijck++) {
		iSizeOfAllNames += strlen(FileNameList[ijck])+1;
	}
	if ( (TotalMemFileSystemSize+sizeof(DIRREC2)*iFileNumber+iSizeOfAllNames) > uncomprMemFileSystemLen ) {
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"memFile is full... unable to add directory, TotalMemFileSystemSize = %u, size needed = %u\n",TotalMemFileSystemSize,TotalMemFileSystemSize+sizeof(DIRREC2)*iFileNumber+iSizeOfAllNames);
#endif
		return(-20);
	}

	long unsigned int lujkTemp = TotalMemFileSystemSize;
	memcpy(uncomprMemFileSystem+4,&lujkTemp,sizeof(long unsigned int)); // copy the offset to the directory to the biginning or the memory array (plus 4 bytes for the memfile format info)
	memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,Directory,sizeof(DIRREC2)*iFileNumber);  // copy the directory to the end of the memory array
	TotalMemFileSystemSize += sizeof(DIRREC2)*iFileNumber;

	for (ijck=0;ijck<iFileNumber;ijck++) {
		DirEntry = (DIRREC2*)(uncomprMemFileSystem+lujkTemp)+ijck;
		DirEntry->NameOffset = (long unsigned int)TotalMemFileSystemSize;  //  point the diretory to the next name location
		memcpy(uncomprMemFileSystem+TotalMemFileSystemSize,FileNameList[ijck],strlen(FileNameList[ijck])+1);  // copy the file name list to the memory array (including the zero byte)
		TotalMemFileSystemSize += strlen(FileNameList[ijck])+1;
		free(FileNameList[ijck]);
		FileNameList[ijck] = NULL;
	}
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"resultant size of Mem File System is = %u\n",TotalMemFileSystemSize);
#endif

	free(FileNameList);
	free(Directory);

	return(iFileNumber);

}



