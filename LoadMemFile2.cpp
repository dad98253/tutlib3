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
		if (debugflag ) dfprintf(fp9, "incompatible zlib version (\"%s\")\nthis is a programming error...\n",myzlibVersion);
		if (debugflag ) dfprintf(fp9,"zlib version %s = 0x%04x, compile flags = 0x%lx\n",ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags());
#endif

		return(1);
	};
#ifdef DEBUG
	if (strcmp(zlibVersion(), myzlibVersion) != 0) {
		if (debugflag ) dfprintf(fp9, "warning: different zlib version (\"%s\")\nthis is a programming problem...\n",myzlibVersion);
	}
	if (debugflag ) dfprintf(fp9,"zlib version %s = 0x%04x, compile flags = 0x%lx\n",ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags());
#endif

	uncompr = ucMemFileSystemData;

	memcpy(&DirOffset,uncompr,sizeof(long unsigned int));
	memcpy(&DirSize,uncompr+sizeof(long unsigned int),sizeof(short unsigned int));
#ifdef DEBUG
	if ( strcmp("tutorDB.txt",szFileName) == 0 ) {
//		int ijkTemp = TUTORUNCOMPFILESIZE+TUTORUNCOMPINDEXSIZE+sizeof(long unsigned int)+sizeof(short unsigned int);
//		if (debugflag ) dfprintf(fp9,"in LoadMemFile, found \"%s\", DirSize = %u, DirOffset = %u, expected directory offset = %i\n",szFileName,DirSize,DirOffset,ijkTemp);
		if (debugflag ) dfprintf(fp9,"in LoadMemFile2, looking for \"%s\", DirSize = %u, DirOffset = %u\n",szFileName,DirSize,DirOffset);
	} else {
		if (debugflag ) dfprintf(fp9,"in LoadMemFile2, DirSize = %u, DirOffset = %u, szFileName = \"%s\"\n",DirSize,DirOffset,szFileName);
	}
#endif

	Dir = (DIRREC2 *)(uncompr+DirOffset);
	int i;
	for (i=0;i<DirSize;i++) {
//		if(strncmp(szFileName,(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wNamelength) == 0 ){
		if(strncmp(szFileName,(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wNamelength) == 0 ){		// find the entry in the memory file system directory
#ifdef DEBUG
			if (debugflag ) dfprintf(fp9,"found \"%s\", file size = %u, file offset = %u\n",(const char *)(uncompr+Dir[i].NameOffset),Dir[i].wFileSize,Dir->FileOffset);
#endif

			break;
		}
	}

	*lpMemFile = (Byte*)calloc((uInt)(Dir[i].wUncompressedFileSize), 1);			// create a new memory (empty) array based on the stored size of the uncompressed file
	if ( *lpMemFile == NULL ) {
		MsgBox("in LoadMemDir... out of memory!\nneed at least %i more bytes to clear this error, but more will likely be required to continue running\n"
		"if you recieved this error, your computer must have a very small memory\nyou likely need several megaBytes more to successfully run My Secret Box!",(int)(Dir[i].wFileSize) );
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"calloc(%u,%i) failed in LoadMemDir\n",(uInt)(Dir[i].wUncompressedFileSize),1);
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
		if (debugflag ) dfprintf(fp9,"uncompressed file size is %u\n",uncomprLen);
#endif
	}

	return(0);
}




