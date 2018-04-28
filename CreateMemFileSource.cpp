/*
 * CreateMemFileSource.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "zlib.h"
#include "tutlib.h"

extern int MsgBox(const char * message, ...);

int CreateMemFileSource() {

	FILE *fSourceCodeFile;

	if ((fSourceCodeFile = fopen("MemSource.h", "w+")) == NULL) {
		MsgBox("unable to open Mem File System source code file for write");
		return(-6);
	}
//	static const char* myVersion = ZLIB_VERSION;
// save the zlibe version in the include file
	fprintf(fSourceCodeFile,"static const char* myzlibVersion = \"%s\";\n",ZLIB_VERSION);

	fprintf(fSourceCodeFile,"unsigned char ucMemFileSystemData[] = {\n");
	unsigned int i;
	for(i=0;i<TotalMemFileSystemSize;i++){
		fprintf(fSourceCodeFile,"0x%x,",*(uncomprMemFileSystem+i));
		if( i > 0 && i%50 == 0 ) fprintf(fSourceCodeFile,"\n");
	}
	fprintf(fSourceCodeFile,"0x00 };\n");
	fclose(fSourceCodeFile);

	return(TotalMemFileSystemSize);
}



