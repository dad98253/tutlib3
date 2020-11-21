/*
 * LoadMemDir.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "zlib.h"
#include "tutlib.h"
#include "tutIndex.h"


int LoadMemDir(Byte ** lpMemDir){
    static const char* myVersion = ZLIB_VERSION;
// the following include defines and initializes the unsigned character array ucMyCompressedData
#include "tutSource.h"

    if ( bMemDirLoaded ) {
#ifdef DEBUG
    	dfprintf2(__LINE__,__FILE__,TRACE,"LoadMemDir called when MemDir is already loaded\n");
#endif
		return(3);
    }
    *lpMemDir = NULL;
    // check to make sure we have the correct ZLIB version...
    if (zlibVersion()[0] != myVersion[0]) {
        fprintf(stderr, "incompatible zlib version\nthis is a programming error...\n");
        return(1);

    } else if (strcmp(zlibVersion(), ZLIB_VERSION) != 0) {
        fprintf(stderr, "warning: different zlib version\nthis is a programming problem...\n");
    }
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"zlib version %s = 0x%04x, compile flags = 0x%lx\n",ZLIB_VERSION, ZLIB_VERNUM, zlibCompileFlags());
#endif

	Byte *uncompr;
	int err;
	uLong comprLen = TUTORTOTALSIZECOMPRESSED + 100;
	uLong uncomprLen = TUTORTOTALSIZEUNCOMPRESSED;

	uncompr = (Byte*)calloc((uInt)uncomprLen, 1);
	if ( uncompr == NULL ) {
		fprintf(stderr, "calloc failed in LoadMemDir... out of memory!\nneed at least %i more bytes to clear this error, but more will likely be required to continue running\n",(int)uncomprLen);
		fprintf(stderr, "if you recieved this error, your computer must have a very small memory\nyou likely need several megaBytes more to successfully run My Secret Box!\n");
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"calloc(%u,%i) failed in LoadMemDir\n",(uInt)uncomprLen,1);
#endif
		return(2);
	} else {
		bMemDirLoaded = true;
	}
// warning, Eclispe may tag the following line with a warning "Symbol ucMyCompressedData could not be resolved"... just ignore, there should be no compile errors...
	err = uncompress(uncompr, &uncomprLen, (Byte *)ucMyCompressedData, comprLen);
	CHECK_ERR(err, "uncompress");

#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"uncompressed file size is %u\n",uncomprLen);
#endif

	*lpMemDir = uncompr;

	return(0);

}




