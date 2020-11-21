/*
 * tutLoadTutMemDB.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "zlib.h"
#include "tutlib.h"

int tutLoadTutMemDB(Byte ** lpTutMemFile){
    static const char* myVersion = ZLIB_VERSION;
// the following include defines and initializes the unsigned character array ucMyCompressedData
//#include "tutSource.h"

    // check to make sure we have the correct ZLIB version...
    if (zlibVersion()[0] != myVersion[0]) {
        fprintf(stderr, "incompatible zlib version\n");
        return(1);

    } else if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) {
        fprintf(stderr, "warning: different zlib version\n");
    }
    /*
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"zlib version %s = 0x%04x, compile flags = 0x%lx\n",ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags());
#endif

	Byte *uncompr;
	int err;
	uLong comprLen = TUTORTOTALSIZECOMPRESSED + 100;
	uLong uncomprLen = TUTORTOTALSIZEUNCOMPRESSED;

	uncompr = (Byte*)calloc((uInt)uncomprLen, 1);
	if ( uncompr == NULL ) {
		fprintf(stderr, "calloc failed in tutLoadTutMemDB... out of memory!\n");
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"calloc(%u,%i) failed in tutLoadTutMemDB\n",(uInt)uncomprLen,1);
#endif
		return(2);
	}
// warning, Eclispe may tag the following line with a warning "Symbol ucMyCompressedData could not be resolved"... just ignore, there should be no compile errors...
	err = uncompress(uncompr, &uncomprLen, (Byte *)ucMyCompressedData, comprLen);
	CHECK_ERR(err, "uncompress");

#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"uncompressed file size is %u\n",uncomprLen);
#endif

	DIRREC *Dir;
//	struct DIRREC {
//		long unsigned int FileOffset;
//		long unsigned int NameOffset;
//	    short unsigned int wFileSize;
//	    short unsigned int wNamelength;
//	 };

	short unsigned int DirSize;
	long unsigned int DirOffset;
	memcpy(&DirOffset,uncompr,sizeof(long unsigned int));
	memcpy(&DirSize,uncompr+sizeof(long unsigned int),sizeof(short unsigned int));
#ifdef DEBUG
	int ijkTemp = TUTORUNCOMPFILESIZE+TUTORUNCOMPINDEXSIZE+sizeof(long unsigned int)+sizeof(short unsigned int);
	dfprintf2(__LINE__,__FILE__,TRACE,"in tutLoadTutMemDB, DirSize = %u, DirOffset = %u, expected offset = %i\n",DirSize,DirOffset,ijkTemp);
//	dfprintf2(__LINE__,__FILE__,TRACE,"in tutLoadTutMemDB, DirSize = , DirOffset = %ul, expected offset = %i\n",DirOffset,ijkTemp);
#endif

	Dir = (DIRREC *)(uncompr+DirOffset);
	if(strncmp("tutorDB.txt",(const char *)(uncompr+Dir->NameOffset),Dir->wNamelength) == 0 ){
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"found \"%s\", file size = %u, file offset = %u\n",(const char *)(uncompr+Dir->NameOffset),Dir->wFileSize,Dir->FileOffset);
#endif
	}

	*lpTutMemFile = uncompr+Dir->FileOffset;
*/
	return(0);
}

