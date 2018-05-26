/*
 * tutCompressAndIndexTutdb.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "zlib.h"
#include "tutlib.h"

extern int MsgBox(const char * message, ...);
extern int inputl(FILE * fp, void * p, int length);

#define TUTCOMPRESSANDINDEXDBG	0
#define getflag(x) (x)

// This routine does a lot of work: It initializes the memory file system, reads the (ascii) tutorial database file,
// iconizes the parts realavant to runtime, indexes it, compresses it, and writes it to the memory file system.
// Note that the tutorial database memory file will always exist and will always be the first file in the memeory file system.
// The routine also creates the tutIndex.h file used to reference the tutorial database after indexing.
// The ascii file is named "fort.1" (yes, tutor started as a fortran program) and must be located in the same directory as
// the passwin executable.

int tutCompressAndIndexTutdb() {
	FILE *fDBFile;
	FILE *fSeqFile;
	FILE *fIndexFile;
	FILE *fDirectFile;
	FILE *fSourceCodeFile;
	FILE *fIncludeFile;
	char buf[BUFFERSIZE];
#define MAXGIDSIZE	80
	char szGID[MAXGIDSIZE];
	int iNumCharsRead;
	int iRecNumber = 0;
	INDEXREC index;
	bool bNewRec = true;
	int iTotRecSize = 0;
//	fpos_t TotalFileSize;
	long unsigned int TotalFileSize=0;
	long unsigned int TotalIndexSize=0;
	Byte *compr, *uncompr;
	Byte *cIndexFile;
	int err;
	uLong comprLen;
	uLong uncomprLen = BUFFERSIZE;
//	char szEOR[] = "\200";
	char szSTR[] = "";
	char szRET[] = "\202";
	char szDEF[] = "\203";
	char szDPI[] = "\204";
	char szTutfile[] = "tutorDB.txt";
	DIRREC Directory[DIRSIZE];
	DIRREC *DirEntry;
/*	struct DIRREC {
		long unsigned int FileOffset;
		long unsigned int NameOffset;
	    short unsigned int wFileSize;
	    short unsigned int wNamelength;
	 };
	#define DIRSIZE		1	*/
	char *FileNameList[DIRSIZE];
	int iFileNumber = 0;

	FileNameList[iFileNumber] = szTutfile;			// the name of the first file will be "tutorDB.txt" (see below)
	TotalFileSize = sizeof(long unsigned int);		// save space for offset to file directory
// open the tutorial database files. fDBFile is the ascii input file that you created. In mode 4, 
// the other tree files are opened in write mode.
// Note that these other three files are no longer used by tutor. They presently are only used to help with debug.
	if ( TutOpenFiles2(4, &fDBFile, &fSeqFile, &fDirectFile, &fIndexFile) ) return(-1);
//                                     xx         xx            xx
	if ((fIncludeFile = fopen("tutIndex.h", "w+")) == NULL) {		// tutIndex.h will contain the defines for the display point id's
		MsgBox("unable to open tutorial index include file for write");
		return(-7);
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"in tutCompressAndIndexTutdb, initial value of uncomprLen = %lu\n",uncomprLen);
#endif
// uncomprLen is initially set to a big number (BUFFERSIZE is set in tutlib03.h).
// NOTE : You will need to set BUFFERSIZE big enough to hold all of the fDBFile file! So, if the 
// fDBFile file size grows, BUFFERSIZE will need to grow, too. BUFFERSIZE is also used to set the
// size of the record index (cIndexFile). So, if you have a lot of small tutorial records, BUFFERSIZE
// might need to be bumped...
// DIRSIZE is also set in tutlib03.h. If the number of files in your memeory file system increases,
// you will need to increase DIRSIZE.
    uncompr  = (Byte*)calloc((uInt)uncomprLen, 1);		
    cIndexFile  = (Byte*)calloc((uInt)uncomprLen, 1);
	short unsigned int suTemp = DIRSIZE;
	memcpy(uncompr+TotalFileSize,&suTemp,sizeof(short unsigned int));   //  save the directory size just after the index to the directory
	TotalFileSize += sizeof(short unsigned int);						// incrememnt TotalFileSize to the next available space


	// start of tutor file data... set directory data
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"file offset to start of tutor file = %u\n",TotalFileSize);
#endif
	(Directory+iFileNumber)->FileOffset = TotalFileSize;				// we will build the directory Directory struct.. Save the offset to the first entry
	(Directory+iFileNumber)->wNamelength = (short unsigned int)strlen(FileNameList[iFileNumber]);	// "tutorDB.txt" is already in the FileNameList

//	Byte * ucStartOfTutotFile;
//	ucStartOfTutotFile = (Byte *)(uncompr+TotalFileSize);		// save a pointer to the first byte in the file , this will be used later to calculate the offset to the index
	int iInitailFileOffset = TotalFileSize;		// save the offset of the start of the tutor file
	short unsigned int* TutorIndexOffset;
	TutorIndexOffset = (short unsigned int*)(uncompr+TotalFileSize);	// save a pointer to the first word in the file for the record index offset, this will be saved later
	TotalFileSize += sizeof(short unsigned int);		// save space for offset to the record index (note that this offset is relative to the start of the tutor file
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"the actual start of the tutor data records is at %u (should be %u)\n",TotalFileSize-iInitailFileOffset,sizeof(short unsigned int));
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
			fprintf(fIncludeFile, "#define %s \t\t\t%i\n",szGID,iRecNumber);	// save the #define to tutIndex.h
			iRecNumber++;
			fprintf(fSeqFile,"%s\n",buf+iFirstChar(buf+6)+6);			// write it out to the sequential file (used for debug only now days...)
#ifdef DEBUG
			dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"*WGID %s (rec #%i) found at offset %u\n",szGID,iRecNumber-1,TotalFileSize-iInitailFileOffset);
#endif
			continue;													// done with WGID data
		}
		if ( strncmp (buf+1,"*WEOR",(size_t)5) == 0 ) {					// found a *WEOR identifier
#ifdef DEBUG
			dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"*WEOR found at offset %u\n",TotalFileSize-iInitailFileOffset);
#endif
//			if(strlen(buf)>1){
//				int iNumCharWrite = sprintf((char *)(uncompr+TotalFileSize),"%s\n",szEOR);
//				iTotRecSize += iNumCharWrite;
//				TotalFileSize += iNumCharWrite;
//			}
			*(uncompr+TotalFileSize) = 0;	// force a zero byte at the end of the record...
			TotalFileSize++;				// (space for the zero byte)
			iTotRecSize++;					// iTotRecSize is the size of the display point record
			index.wUncompressedsize = iTotRecSize;	// save the size of the record in the index struct (fyi, INDEXREC is defined in tutlib01.h)
#ifdef DEBUG
			dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"[tutComp...]iTotRecSize = %u, index offset (rel to start of index) = %u\n",iTotRecSize,TotalIndexSize);
#endif
			iTotRecSize = 0;				// get ready for next record
			memcpy(cIndexFile+TotalIndexSize, &index, sizeof(index) );		// copy the index struct to the cIndexFile temp memory file
			TotalIndexSize += sizeof(index);								// bump the next free cIndexFile location
			index.wUncompressedsize = 0;									// initialize the next record size to zero
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
			// iconize the *WRET, *WDEF, and *WDPI identifiers
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
		*(uncompr+TotalFileSize) = 0;	// the following just dups what is done above when a *WEOR is fond...
		TotalFileSize++;
		iTotRecSize++;
		index.wUncompressedsize = iTotRecSize;
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"last record saved... iTotRecSize = %u, index offset (rel to start of index) = %u\n",iTotRecSize,TotalIndexSize);
#endif
		iTotRecSize = 0;
		memcpy(cIndexFile+TotalIndexSize, &index, sizeof(index) );
		TotalIndexSize += sizeof(index);
		index.wUncompressedsize = 0;
		bNewRec = true;
	}
// close all of the tutorial database files (mode (3 here) is hisotical and has no effect now but never the less must be 1,2,3,or 4)
	if ( TutCloseFiles2(3, fDBFile, fSeqFile, fDirectFile, fIndexFile) ) return(-2);

	//	struct DIRREC {
	//		long unsigned int FileOffset;
	//		long unsigned int NameOffset;
	//	    short unsigned int wFileSize;
	//	    short unsigned int wNamelength;
	//	 };
	//	 char *FileNameList[DIRSIZE];
	*TutorIndexOffset = (short unsigned int)(TotalFileSize-iInitailFileOffset);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"TotalFileSize = %lu, TotalIndexSize = %lu, iRecNumber = %i\n",TotalFileSize-iInitailFileOffset,TotalIndexSize,iRecNumber);
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"calculated  TutorIndexOffset = %u\n",*TutorIndexOffset);
#endif

	memcpy(uncompr+TotalFileSize,&iRecNumber,sizeof(iRecNumber));		// tack the number of records onto the memory file
	memcpy(uncompr+TotalFileSize+sizeof(iRecNumber),cIndexFile,TotalIndexSize);		// tack on the record index after that
	TotalIndexSize += sizeof(iRecNumber);
	uncomprLen = TotalFileSize + TotalIndexSize;		// calculate the size of the file. uncomprLen will eventually be passed to the compression routine
	(Directory+iFileNumber)->wFileSize = (unsigned short)(uncomprLen - iInitailFileOffset);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"file #%i file size = %u\n",iFileNumber,(Directory+iFileNumber)->wFileSize);
#endif
	iFileNumber++;		// increment the number of files in the directory
	/// if adding other files, do it here...
	// for(i=1;i<DIRSIZE;i++) add file + update Directory offset & len
	// uncomprLen += whatever is added

	long unsigned int lujkTemp = uncomprLen;
	memcpy(uncompr,&lujkTemp,sizeof(long unsigned int)); // copy the offset to the file directory to the biginning or the memory array
	memcpy(uncompr+uncomprLen,Directory,sizeof(DIRREC)*iFileNumber);  // copy the file directory to the end of the memory array
	uncomprLen += sizeof(DIRREC)*iFileNumber;		// bump the size of the array by the added directory space
	int ijck;										// now wee need to add on the file names (pointed to within the directory)
	for (ijck=0;ijck<iFileNumber;ijck++) {
		DirEntry = (DIRREC*)(uncompr+lujkTemp)+ijck;
		DirEntry->NameOffset = (long unsigned int)uncomprLen;  //  point the diretory to the next available name location
		memcpy(uncompr+uncomprLen,FileNameList[ijck],strlen(FileNameList[ijck])+1);  // copy the next file name to the memory array (including the zero byte)
		uncomprLen += strlen(FileNameList[ijck])+1;				// bump the next available name location
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"expected uncomprLen = %u\n",uncomprLen);
#endif
	comprLen = uncomprLen + 1000;  // add a little extra just in case the array doesn't shrink properly (possible for very small and/or uncompressable files)
    compr    = (Byte*)calloc((uInt)comprLen, 1);		// allocate space for the compressed array

/*
    int iByteOffset = 0;
    int iNumBytesRead;
	rewind(fIndexFile);
	rewind(fDirectFile);
    while(1) {
    	if ( ( iNumBytesRead = fread(&index,sizeof(index),1,fIndexFile) ) != 1 ) {
    		if ( !feof(fIndexFile) ) {
    			MsgBox("error reading IndexFile, iNumBytesRead = %i",iNumBytesRead);
    		} else {
    			break;
    		}
    	}
    	fread(uncompr+iByteOffset,index.wUncompressedsize,1,fDirectFile);
    	iByteOffset += index.wUncompressedsize;
    }  */

//	err = compress(compr, &comprLen, uncompr, iByteOffset);
	err = compress(compr, &comprLen, uncompr, uncomprLen);  // note that comprLen is both input and output. It checks to make sure not to over index the array...
	 CHECK_ERR(err, "compress");
// open the source code file. We are going to write the compressed data array to it in a format that can be used to initialize a byte array in c via MemSource.h
// when you are happy with the file creation, Just copy tutSource.61 and tutIndex.h to your tutlib2 build directory and rename tutSource.61 to MemSource.h
		if ((fSourceCodeFile = fopen("tutSource.61", "w+")) == NULL) {
			MsgBox("unable to open tutorial source code file for write");
			return(-6);
		}

	fprintf(fSourceCodeFile,"unsigned char ucMyCompressedData[] = {\n");		// write out the first line
	unsigned int i;
	for(i=0;i<comprLen;i++){
		fprintf(fSourceCodeFile,"0x%x,",*(compr+i));							// write out the entire compressed file as hex byte data
		if( i > 0 && i%50 == 0 ) fprintf(fSourceCodeFile,"\n");					// 50 bytes to a line
	}
	fprintf(fSourceCodeFile,"0x00 };\n");										// add one last byte
	fclose(fSourceCodeFile);													// and close the file

// now we will test the compressed array to make sure we can uncompress it correctly
	 strcpy((char*)uncompr, "garbage");											// write some garbage to the uncompress location (should be overwritten)
// uncompress our compressed data
	 err = uncompress(uncompr, &uncomprLen, compr, comprLen);
	 CHECK_ERR(err, "uncompress");

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"uncomprLen after uncompress call = %lu\n",uncomprLen);
	dfprintf(__LINE__,__FILE__,TUTCOMPRESSANDINDEXDBG,"comprLen = %lu, compresion ratio = %f\n",comprLen,(float)comprLen/(float)uncomprLen);
#endif
// tack some more stuff onto the end of the tutIndex.h file. it can be used to check the uncompressed array at runtime
	fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORUNCOMPFILESIZE",TotalFileSize-sizeof(long unsigned int)-sizeof(short unsigned int));
	fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORUNCOMPINDEXSIZE",TotalIndexSize);
	fprintf(fIncludeFile, "#define %s \t\t%i\n","TUTORNUMERRECORDS",iRecNumber);
	fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORTOTALSIZEUNCOMPRESSED",uncomprLen);
	fprintf(fIncludeFile, "#define %s \t\t%lu\n","TUTORTOTALSIZECOMPRESSED",comprLen);
	fclose(fIncludeFile);		// close tutIndex.h
// free up all of our allocated memory arrays
	free(compr);
	free(uncompr);
	free(cIndexFile);
// Note: if more files are added, then you may need to free up the pointers in FileNameList[]. As it is, the only name in the list is at FileNameList[0]
// and it points to "tutorDB.txt" which is a const char. So do NOT free FileNameList[0]!
	return(iRecNumber);
}



