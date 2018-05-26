/*
 * LoadMemFile.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"
#include "tutIndex.h"

int LoadMemFile(Byte ** lpMemDir, const char * szFileName, Byte ** lpMemFile, short unsigned int * FileSize) {
	Byte *uncompr;
	DIRREC *Dir;
//	struct DIRREC {
//		long unsigned int FileOffset;
//		long unsigned int NameOffset;
//	    short unsigned int wFileSize;
//	    short unsigned int wNamelength;
//	 };
	short unsigned int DirSize;
	long unsigned int DirOffset;

	uncompr = *lpMemDir;

	memcpy(&DirOffset,uncompr,sizeof(long unsigned int));
	memcpy(&DirSize,uncompr+sizeof(long unsigned int),sizeof(short unsigned int));
#ifdef DEBUG
	if ( strcmp("tutorDB.txt",szFileName) == 0 ) {
		int ijkTemp = TUTORUNCOMPFILESIZE+TUTORUNCOMPINDEXSIZE+sizeof(long unsigned int)+sizeof(short unsigned int);
		dfprintf(__LINE__,__FILE__,TRACE,"in LoadMemFile, found \"%s\", DirSize = %u, DirOffset = %u, expected directory offset = %i\n",szFileName,DirSize,DirOffset,ijkTemp);
	} else {
		dfprintf(__LINE__,__FILE__,TRACE,"in LoadMemFile, DirSize = %u, DirOffset = %u, szFileName = \"%s\"\n",DirSize,DirOffset,szFileName);
	}
#endif

	Dir = (DIRREC *)(uncompr+DirOffset);
	int i;
	for (i=0;i<DirSize;i++) {
		if(strncmp(szFileName,(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wNamelength) == 0 ){
#ifdef DEBUG
			dfprintf(__LINE__,__FILE__,TRACE,"found \"%s\", file size = %u, file offset = %u\n",(const char *)(uncompr+Dir->NameOffset),Dir->wFileSize,Dir->FileOffset);
#endif
			break;
		}
	}
	*lpMemFile = uncompr+Dir[i].FileOffset;
	*FileSize = Dir[i].wFileSize;

	return(0);
}



