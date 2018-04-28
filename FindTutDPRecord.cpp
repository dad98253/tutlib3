/*
 * FindTutDPRecord.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"


int FindTutDPRecord(int iDisplayPointID){
	long unsigned int uRecOffset;

	if ( lpTutMemFile == NULL ) {  // check if the tutor memory file has been loaded
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"in tutDisplayTextPromptx, lpTutMemFile == NULL\n");
#endif
		return(-2);
	}
	short unsigned int IndexOffset;
	IndexOffset = *((short unsigned int *)lpTutMemFile);	// the first word in the file should be the offset to the record index
	short unsigned int * pToIndex;
	int iRecTotal = *(int *)(lpTutMemFile+IndexOffset);		// get the size of the index (in first word of index)
	pToIndex = (short unsigned int *)(lpTutMemFile+IndexOffset+sizeof(int));  // calculate a pointer to the start of the index entries
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"in tutDisplayTextPromptx, lpTutMemFile == 0x%x\n",lpTutMemFile);
	if (debugflag ) dfprintf(fp9,"IndexOffset = %u, pointer to index =0x%x, iRecTotal = %i\n",IndexOffset,pToIndex,iRecTotal);
#endif
	if ( iDisplayPointID < 0 || iDisplayPointID > iRecTotal ) {
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"bad iDisplayPointID (%i)\n",iDisplayPointID);
#endif
		return(-1);
	}
	INDEXREC *pIndexRec;
	pIndexRec = (INDEXREC *)pToIndex+iDisplayPointID;
// struct INDEXREC {
//	long unsigned int pos;
//   short unsigned int wUncompressedsize;
// };

	uRecOffset = pIndexRec->pos;

#ifdef DEBUG
	short unsigned int uRecLen;
	//	uRecLen = *(short unsigned int *)(pIndexRec->wUncompressedsize);
	uRecLen = pIndexRec->wUncompressedsize;
	if (debugflag ) dfprintf(fp9,"in tutDisplayTextPromptx, iDisplayPointID = %i found at uRecOffset = %u, with uRecLen = %u\nSearching for null byte in record...",iDisplayPointID,uRecOffset,uRecLen);
	if (strlen((const char *)(lpTutMemFile+uRecOffset)) > 1000 ) {
		if (debugflag ) dfprintf(fp9,"in tutDisplayTextPromptx, Dp Record size appears to be way too long!\n");
		return(8);
	}
	if (strlen((const char *)(lpTutMemFile+uRecOffset)) != (unsigned int)(uRecLen-1) ) {
		if (debugflag ) dfprintf(fp9,"in tutDisplayTextPromptx, Dp Record size in the index is different from the strlen() size, uRecLen = %u, strlen() = %i\n",uRecLen,strlen((const char *)(lpTutMemFile+uRecOffset)));;
		return(9);
	}
	if (debugflag ) dfprintf(fp9,"iDisplayPointID = %i looks like this:\n%s",iDisplayPointID,(const char *)(lpTutMemFile+uRecOffset));
#endif

 return((int)uRecOffset);
}



