/*
 * LoadMemFile2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */
#include <string.h>
#include "zlib.h"
#include "tutlib.h"


// int LoadMemFile(Byte ** lpMemDir, const char * szFileName, Byte ** lpMemFile, short unsigned int * FileSize) {
int LoadMemFile2(const char * szFileName, Byte ** lpMemFile, short unsigned int * FileSize) {
	// the following defines and initializes the unsigned character array ucMemFileSystemData
#ifdef GENMEMFILE
	unsigned char *ucMemFileSystemData;					// comment this out when including the MemSource.h file
    static const char* myzlibVersion = ZLIB_VERSION;	// comment this out when including the MemSource.h file
    ucMemFileSystemData = uncomprMemFileSystem;
#else	// GENMEMFILE
#include "MemSource.h"
#endif	// GENMEMFILE

	Byte *uncompr;
	DIRREC2 *Dir;

	//	struct DIRREC2 {
	//		long unsigned int FileOffset;
	//		long unsigned int NameOffset;
	//	    short unsigned int wFileSize;
	//	    short unsigned int wUncompressedFileSize;
	//	    short unsigned int wNamelength;
	//	    Byte flags;
	//	 };
	//	 char *FileNameList[uMaxDirSize];
	short unsigned int DirSize;
	long unsigned int DirOffset;

	if (ucMemFileSystemData == NULL) {
		MsgBox("in LoadMemDir... ucMemFileSystemData is a NULL pointer...\nThis is a programming error!");
//		return(-3);
	}

   // check to make sure we have the correct ZLIB version...
	if (zlibVersion()[0] != myzlibVersion[0]) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "incompatible zlib version (\"%s\")\nthis is a programming error...\n",myzlibVersion);
		dfprintf(__LINE__,__FILE__,TRACE,"zlib version %s = 0x%04x, compile flags = 0x%lx\n",ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags());
#endif

		return(1);
	};
#ifdef DEBUG
	if (strcmp(zlibVersion(), myzlibVersion) != 0) {
		dfprintf(__LINE__,__FILE__,TRACE, "warning: different zlib version (\"%s\")\nthis is a programming problem...\n",myzlibVersion);
	}
	dfprintf(__LINE__,__FILE__,TRACE,"zlib version %s = 0x%04x, compile flags = 0x%lx\n",ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags());
#endif

	uncompr = ucMemFileSystemData;
	// verify that the data saved in the memfile system is in the format that we expect...
#define HEADERSIZE	4
	unsigned char bOneByte;
	unsigned char bMemFileFormat = 0x95;
	memcpy(&bOneByte,uncompr+0,1);		//  get memfile format
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE, "MemFile format = 0x%x\n",bOneByte);
#endif
	if ( bOneByte != bMemFileFormat ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "MemFile format is incomatible! Expected format == 0x%x\n",bMemFileFormat);
#endif
		return(-21);
	}
	unsigned char bShortIntSize;
	memcpy(&bShortIntSize,uncompr+1,1);		//  get size of short int
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE, "MemFile size of short int = 0x%x\n",bShortIntSize);
#endif
	if ( bShortIntSize != sizeof(short unsigned int) ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "MemFile format is incomatible! Expected short int size == 0x%x\n",sizeof(short unsigned int));
#endif
		return(-22);
	}
	unsigned char bIntSize;
	memcpy(&bIntSize,uncompr+2,1);		//  get size of short int
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE, "MemFile size of int       = 0x%x\n",bIntSize);
#endif
	if ( bIntSize != sizeof(unsigned int) ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "MemFile format is incomatible! Expected int size == 0x%x\n",sizeof(unsigned int));
#endif
		return(-23);
	}
	unsigned char bLongIntSize;
	memcpy(&bLongIntSize,uncompr+3,1);		//  get size of short int
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE, "MemFile size of long int  = 0x%x\n",bLongIntSize);
#endif
	if ( bLongIntSize != sizeof(long unsigned int) ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE, "MemFile format is incomatible! Expected long int size == 0x%x\n",sizeof(long unsigned int));
#endif
		return(-24);
	}

	memcpy(&DirOffset,uncompr+HEADERSIZE,sizeof(long unsigned int));		// get the offest to the directory data
	memcpy(&DirSize,uncompr+sizeof(long unsigned int)+HEADERSIZE,sizeof(short unsigned int));	// get the size of the directory
#ifdef DEBUG
	if ( strcmp("tutorDB.txt",szFileName) == 0 ) {
//		int ijkTemp = TUTORUNCOMPFILESIZE+TUTORUNCOMPINDEXSIZE+sizeof(long unsigned int)+sizeof(short unsigned int);
//		dfprintf(__LINE__,__FILE__,TRACE,"in LoadMemFile, found \"%s\", DirSize = %u, DirOffset = %u, expected directory offset = %i\n",szFileName,DirSize,DirOffset,ijkTemp);
		dfprintf(__LINE__,__FILE__,TRACE,"in LoadMemFile2, looking for \"%s\", DirSize = %u, DirOffset = %u\n",szFileName,DirSize,DirOffset);
	} else {
		dfprintf(__LINE__,__FILE__,TRACE,"in LoadMemFile2, DirSize = %u, DirOffset = %u, szFileName = \"%s\"\n",DirSize,DirOffset,szFileName);
	}
#endif

	Dir = (DIRREC2 *)(uncompr+DirOffset);
	int i;
	for (i=0;i<DirSize;i++) {
//		if(strncmp(szFileName,(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wNamelength) == 0 ){
		if(strncmp(szFileName,(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wNamelength) == 0 ){		// find the entry in the memory file system directory
#ifdef DEBUG
			dfprintf(__LINE__,__FILE__,TRACE,"found \"%s\", file size = %u, file offset = %u\n",(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wFileSize,Dir->FileOffset);
#endif

			break;
		}
	}

	*lpMemFile = (Byte*)calloc((uInt)(Dir[i].wUncompressedFileSize), 1);			// create a new memory (empty) array based on the stored size of the uncompressed file
	if ( *lpMemFile == NULL ) {
		MsgBox("in LoadMemDir... out of memory!\nneed at least %i more bytes to clear this error, but more will likely be required to continue running\n"
		"if you recieved this error, your computer must have a very small memory\nyou likely need several megaBytes more to successfully run My Secret Box!",(int)(Dir[i].wFileSize) );
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"calloc(%u,%i) failed in LoadMemDir\n",(uInt)(Dir[i].wUncompressedFileSize),1);
#endif
		return(-2);
	}

	if (  uncompr+Dir[i].flags == 0 ) {												// check if the file is compressed (ziped) or not
		memcpy(*lpMemFile,uncompr+Dir[i].FileOffset,Dir[i].wUncompressedFileSize);	// copy the file directly from the directory to the new memory array
		*FileSize = Dir[i].wFileSize;
	} else {
		int err;
		uLong comprLen = Dir[i].wFileSize;
		uLong uncomprLen = Dir[i].wUncompressedFileSize;

		err = uncompress(*lpMemFile, &uncomprLen, uncompr+Dir[i].FileOffset, comprLen);    //  unzip the compressed memory file to the new memory array
		CHECK_ERR(err, "uncompress");

#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"uncompressed file size is %u\n",uncomprLen);
#endif
	}

	return(0);
}




