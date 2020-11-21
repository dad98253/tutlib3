/*
 * tutIndexTutdb.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "zlib.h"
#include "tutlib.h"

extern int MsgBox(const char * message, ...);
extern int inputl(FILE * fp, void * p, int length);


#define TUTINDEXDBDBG	0
#define getflag(x) (x)

// This routine reads the (ascii) tutorial database file, iconizes the parts realavant to runtime, and indexes it.
// The routine also optionally creates the include file used to reference the tutorial database after indexing.
// The include data is written to the file name specified by szIncludeFileName. If the szIncludeFileName pointer is NULL,
// then no include file data is produced.
// The input ascii file is named "fort.1" (yes, tutor started as a fortran program). TutOpenFiles2 expects it to be located
// in the same directory as the passwin executable.

int tutIndexTutdb(Byte ** lpMemFile, int * iFileSize, char * szIncludeFileName ) {
	FILE *fDBFile;
	FILE *fSeqFile;
	FILE *fIndexFile;
	FILE *fDirectFile;
	FILE *fIncludeFile;
#define BUFFERSIZE	50000
	char buf[BUFFERSIZE];
#define MAXGIDSIZE	80
	char szGID[MAXGIDSIZE];
	int iNumCharsRead;
	int iRecNumber = 0;
	INDEXREC2 index;
	bool bNewRec = true;
	int iTotRecSize = 0;
//	fpos_t TotalFileSize;
	long unsigned int TotalFileSize=0;
	long unsigned int TotalIndexSize=0;
	Byte *uncompr;
	Byte *cIndexFile;
	uLong uncomprLen = BUFFERSIZE;
//	char szEOR[] = "\200";
	char szSTR[] = "";
	char szRET[] = "\202";
	char szDEF[] = "\203";
	char szDPI[] = "\204";

// open the tutorial database files. fDBFile is the ascii input file that you created. In mode 4, 
// the other tree files are opened in write mode.
// Note that these other three files are no longer used by tutor during runtime. They presently
// are only used to help with debug.
	if ( TutOpenFiles2(4, &fDBFile, &fSeqFile, &fDirectFile, &fIndexFile) ) return(-1);
//                                     xx         xx            xx
	if ( szIncludeFileName != NULL ) {		// szIncludeFileName will contain the defines for the display point id's
		if ((fIncludeFile = fopen(szIncludeFileName, "w+")) == NULL) {
			MsgBox("unable to open tutorial index include file \"%s\" for write",szIncludeFileName);
			return(-7);
		}
	}
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"in tutIndexTutdb, initial value of uncomprLen = %lu\n",uncomprLen);
#endif
// uncomprLen is initially set to a big number (BUFFERSIZE is set in tutlib03.h).
// NOTE : You will need to set BUFFERSIZE big enough to hold all of the memory file system! So, if the 
// file system size grows, BUFFERSIZE will need to grow, too. BUFFERSIZE is also used to set the
// size of the record index (cIndexFile). So, if you have a lot of small tutorial records, BUFFERSIZE
// might need to be bumped...
// DIRSIZE is also set in tutlib03.h. If the number of files in your memeory file system increases,
// you will need to increase DIRSIZE.
    uncompr  = (Byte*)calloc((uInt)uncomprLen, 1);		// this address will eventually be returned via the lpMemFile calling argument
    cIndexFile  = (Byte*)calloc((uInt)uncomprLen, 1);


	// start of tutor file data...

//	Byte * ucStartOfTutotFile;
//	ucStartOfTutotFile = (Byte *)(uncompr+TotalFileSize);		// save a pointer to the first byte in the file , this will be used later to calculate the offset to the index
	int iInitailFileOffset = TotalFileSize;		// save the offset of the start of the tutor file
	short unsigned int* TutorIndexOffset;
	TutorIndexOffset = (short unsigned int*)(uncompr+TotalFileSize);	// save a pointer to the first word in the file for the record index offset, this will be saved later
	TotalFileSize += sizeof(short unsigned int);		// save space for offset to the record index (note that this offset is relative to the start of the tutor file
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"the actual start of the tutor data records is at %u (should be %u)\n",TotalFileSize-iInitailFileOffset,sizeof(short unsigned int));
#endif
	while(1) {
		if( ( iNumCharsRead = inputl(fDBFile, buf, BUFFERSIZE) ) < 0 ) break;	// read one line of the tutorial database file from disk until we hit end of file
//		if (bNewRec) fgetpos(fDirectFile, &index.pos );
// bNewRec refers to the group of lines that define the display point data: They normally start with a start with a *WGID identifier and always end with a *WEOR.
// Note that inserting a line after a *WEOR and before a *WGID may have unpredictable results...
// bNewRec is initially set true (for the first "record") and reset to true each time a *WEOR identifier is encountered.
		if (bNewRec) index.pos = TotalFileSize-iInitailFileOffset;
		bNewRec = false;
		if ( strncmp (buf+1,"*WGID",(size_t)5) == 0 ) {
			if (FixGIDName(buf+6,szGID,MAXGIDSIZE)) return(-10);		// found a *WGID... the rest of the line will be the GID. Historically, a GID could containe blanks.
																		// FixGIDName converts blanks to underscores so that we can create #define's for them.
																		// btw, it also changes -'s to _'s
			if (strncmp(szGID,"TUT",3) == 0) break;		// GIDs that start with "TUT" were historically tutor error messages... they are not be saved in the memory file
			if ( szIncludeFileName != NULL ) fprintf(fIncludeFile, "#define %s \t\t\t%i\n",szGID,iRecNumber);
			iRecNumber++;
#ifdef DEBUG
			int iNbRet =
#endif
						fprintf(fSeqFile,"%s\n",buf+iFirstChar(buf+6)+6);			// write it out to the sequential file (used for debug only now days...)
#ifdef DEBUG
			dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"*WGID %s (rec #%i) found at offset %u, %i bytes written to fSeqFile\n",szGID,iRecNumber-1,TotalFileSize-iInitailFileOffset,iNbRet);
#endif
			continue;													// done with WGID data
		}
		if ( strncmp (buf+1,"*WEOR",(size_t)5) == 0 ) {					// found a *WEOR identifier
#ifdef DEBUG
			dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"*WEOR found at offset %u\n",TotalFileSize-iInitailFileOffset);
#endif
//			if(strlen(buf)>1){
//				int iNumCharWrite = sprintf((char *)(uncompr+TotalFileSize),"%s\n",szEOR);
//				iTotRecSize += iNumCharWrite;
//				TotalFileSize += iNumCharWrite;
//			}
			*(uncompr+TotalFileSize) = 0;	// force a zero byte at the end of the record...
			TotalFileSize++;				// (space for the zero byte)
			iTotRecSize++;					// iTotRecSize is the size of the display point record
			index.wRecordSize = iTotRecSize;	// save the size of the record in the index struct (fyi, INDEXREC2 is defined in tutlib01.h)
#ifdef DEBUG
			dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"iTotRecSize = %u, index offset (rel to start of index) = %u\n",iTotRecSize,TotalIndexSize);
#endif
			iTotRecSize = 0;				// get ready for next record
			memcpy(cIndexFile+TotalIndexSize, &index, sizeof(index) );		// copy the index struct to the cIndexFile temp memory file
			TotalIndexSize += sizeof(index);								// bump the next free cIndexFile location
			index.wRecordSize = 0;											// initialize the next record size to zero
			bNewRec = true;													// the next time thru the while, a new index.pos will be saved
			continue;
		}
		if ( strncmp (buf+1,"*WPEN",(size_t)5) == 0 ) continue;				// we ignore all of these identifiers... they are not used during runtime
		if ( strncmp (buf+1,"*WNDP",(size_t)5) == 0 ) continue;				// historically, they were used for an automated documentation generator
		if ( strncmp (buf+1,"*WBST",(size_t)5) == 0 ) continue;				// that has not beed converted from fortran to c
		if ( strncmp (buf+1,"*WBEN",(size_t)5) == 0 ) continue;				// at some point it might be nice to reintroduce these and use them for
		if ( strncmp (buf+1,"*WPEC",(size_t)5) == 0 ) continue;				// a runtime "help" system!
		if(strlen(buf)>1) {
		/*	 *WRET F 1 INT 1 OR 2
			 *WDEF 2
			 *WDPI [DISPLAY OPTIONS]
		*/
			char * szTempLab = szSTR;											// szSTR = ""
			if ( strncmp (buf+1,"*WRET",(size_t)5) == 0 ) szTempLab = szRET;	// szRET = "\202"
			if ( strncmp (buf+1,"*WDEF",(size_t)5) == 0 ) szTempLab = szDEF;	// szDEF = "\203"
			if ( strncmp (buf+1,"*WDPI",(size_t)5) == 0 ) szTempLab = szDPI;	// szDPI = "\204"
			int iStartOutput = 1;												// anything that does not start with one of these identifiers is
																				// part of the tutorial display point. copy it as is to the memory file
			if ( szTempLab != szSTR ) iStartOutput = 6;							// skip over the (iconized) identifier
			int iNumCharWrite = 0;
			if ( StripSingleQuotes(buf+iStartOutput) >= 0 ) {					// historically, the substitution strings were contained within single
																				// quotes. we don't do that any more, so they are striped out to permit
																				// use of old format database files. However, this means that we can't
																				// have single quotes in out text (e.g., can't -> cant). At some point
																				// we need to abandon backward compatability and eliminate this
																				// StripSingleQuotes call...
				iNumCharWrite = sprintf((char *)(uncompr+TotalFileSize),"%s%s\n",szTempLab,buf+iStartOutput);		// copy the line to the memory file
				iTotRecSize += iNumCharWrite;									// bump both the record size and the total memory file size
				TotalFileSize += iNumCharWrite;
			}																	// note: if StripSingleQuotes returns an error, nothing is copied...
		}
	}		// end of while (end of tutorial database file)
	if ( iTotRecSize != 0 ) {			// take care of the last record (only if the last *WEOR is missing)
										// the following just dups what is done above when a *WEOR is found...
		*(uncompr+TotalFileSize) = 0;	// force a zero byte at the end of the record...
		TotalFileSize++;
		iTotRecSize++;
		index.wRecordSize = iTotRecSize;
#ifdef DEBUG
		dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"last record saved... iTotRecSize = %u, index offset (rel to start of index) = %u\n",iTotRecSize,TotalIndexSize);
#endif
		iTotRecSize = 0;
		memcpy(cIndexFile+TotalIndexSize, &index, sizeof(index) );
		TotalIndexSize += sizeof(index);
		index.wRecordSize = 0;
		bNewRec = true;
	}
// close all of the tutorial database files (mode (3 here) is hisotical and has no effect now but never the less must be 1,2,3,or 4)
	if ( TutCloseFiles2(3, fDBFile, fSeqFile, fDirectFile, fIndexFile) ) return(-2);

	*TutorIndexOffset = (short unsigned int)(TotalFileSize-iInitailFileOffset);
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"TotalFileSize = %lu, TotalIndexSize = %lu, iRecNumber = %i\n",TotalFileSize-iInitailFileOffset,TotalIndexSize,iRecNumber);
	dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"calculated  TutorIndexOffset = %u\n",*TutorIndexOffset);
#endif

	memcpy(uncompr+TotalFileSize,&iRecNumber,sizeof(iRecNumber));		// tack the number of records onto the memory file
	memcpy(uncompr+TotalFileSize+sizeof(iRecNumber),cIndexFile,TotalIndexSize);		// tack on the record index after that
	TotalIndexSize += sizeof(iRecNumber);
	uncomprLen = TotalFileSize + TotalIndexSize;		// calculate the size of the file. uncomprLen will returned via the iFileSize calling argument
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TUTINDEXDBDBG,"total size of tutor file (uncompressed) = %lu\n",uncomprLen);
#endif
// tack some more stuff onto the end of the include file. it can be used to check the uncompressed array at runtime
	if ( szIncludeFileName != NULL ) {
		fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORUNCOMPFILESIZE",TotalFileSize);
		fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORUNCOMPINDEXSIZE",TotalIndexSize);
		fprintf(fIncludeFile, "#define %s \t\t%i\n","TUTORNUMERRECORDS",iRecNumber);
		fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORTOTALSIZEUNCOMPRESSED",uncomprLen);
		fclose(fIncludeFile);		// close the include file
	}

	*lpMemFile = uncompr;
	*iFileSize = (int)uncomprLen;
	free(cIndexFile);	// free up the allocated memory used to creat our index array

	return(iRecNumber);
}




