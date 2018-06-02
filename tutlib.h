/*
 * tutlib.h
 *
 *  Created on: Feb 10, 2016
 *      Author: dad
 */

#ifndef TUTLIB_H_
#define TUTLIB_H_

#include "tutlib03.h"

#ifdef OLDCOPY     //////////////////////////////////////////////////   ignore !!!  ////////////////////////////////////////////////
extern int tutio (char *szInputFile, char *szOutputFile, char * szTutlogFilenameParm , char * szTutDebugFilenameParm , char * szTutdbFilenameParm
		, char * szTutdbDAFilenameParm, char * szTutdbINDFilenameParm);

extern int tutDisplayPrompt (const char * szDispPtName, int iNumArg, ...);
extern int tutDisplayTextPrompt (const char * szDispPtName, int iNumArg, ...);
extern int tutRecursiveInputInt (const char * szDispPtNameLong, const char * szDispPtNameShort, int* Data, int iSizeData, int* iNumData, int iNumArg, ...);
extern int tutConvertString (char * szString, int iStringSize);
extern float tutFetchData (int iIndex);
extern int tutPrintString (const char * szText);
extern int EchoOff();
extern int EchoOn();
extern int tutClose();
extern int GenRepString(char * A, const char c, int n);
#endif           ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


#ifndef TUTLIBINCMACS_H_
#define TUTLIBINCMACS_H_

#ifndef TUTLIBMAIN
#define EXTERN		extern
#define INITIZERO
#define INITSZERO
#define INITBOOLFALSE
#define INITBOOLTRUE
#define INITNULL
#define INITBUFFERSIZE
#define INITDIRSIZE
#else
#define EXTERN
#define INITIZERO	=0
#define INITSZERO	={0}
#define INITBOOLFALSE	=false
#define INITBOOLTRUE	=true
#define INITNULL	=NULL
#define INITBUFFERSIZE	=BUFFERSIZE
#define INITDIRSIZE		=DIRSIZE
#endif


#if defined(DOSWIN32) || defined(_MAC)
#define cdecl _cdecl
#ifndef CDECL
#define CDECL _cdecl
#endif
#else
#define cdecl
#ifndef CDECL
#define CDECL cdecl
#endif
#ifndef FAR
#define FAR
#endif
#endif

#endif  // TUTLIBINCMACS_H_

// note that if zlib is used in a module that include tutlib.h, the zlib.h include MUST precede the tutlib.h include !!!
#ifndef ZCONF_H
typedef unsigned char  Byte;  /* 8 bits */
typedef unsigned int   uInt;  /* 16 bits or more */
typedef unsigned long  uLong; /* 32 bits or more */
#define Z_OK            0
#endif

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
extern int MsgBox(const char * message, ...);
#include "tutlib01.h"
#include "tutIndex.h"


EXTERN int tutio (char *szInputFile, char *szOutputFile, char * szTutlogFilenameParm , char * szTutDebugFilenameParm , char * szTutdbFilenameParm
		, char * szTutdbDAFilenameParm, char * szTutdbINDFilenameParm);
extern int tutIsTty();

EXTERN int tutDisplayPrompt (const char * szDispPtName, int iNumArg, ...);
EXTERN int tutDisplayTextPrompt (const char * szDispPtName, int iNumArg, ...);
EXTERN int tutRecursiveInputInt (const char * szDispPtNameLong, const char * szDispPtNameShort, int* Data, int iSizeData, int* iNumData, int iNumArg, ...);
EXTERN int tutConvertString (char * szString, int iStringSize);
EXTERN float tutFetchData (int iIndex);
EXTERN int tutPrintString (const char * szText);
EXTERN int EchoOff();
EXTERN int EchoOn();
EXTERN int tutClose();
EXTERN int GenRepString(char * A, const char c, int n);
EXTERN int FindTutDPRecord(int iDisplayPointID);
EXTERN int ParseDPRecord ( char * lpsEditedResource , int iLenResource , DPINFO *DPInfo , char ** szDefaultInput , char ** szDPIDString , char ** szDPText );
EXTERN int tutPrompt(TUTSTATUSDATA *lpTStatus, char * szDPIDString,char *szDefaultInput,char *szDPText);
EXTERN bool tutProcessInput(TUTSTATUSDATA *lpTStatus, DPINFO *DPInfo, char * szDefaultInput, int iCallerID);
EXTERN int tutDisplayWorker ( int iDisplayPointID, int iCallerID, int NumReplacements , va_list ap );
EXTERN int tutDisplayWorkerL2 ( int iDisplayPointID, int iCallerID, int NumReplacements , va_list ap );
EXTERN int tutGetNextLineInput( TUTSTATUSDATA *lpTStatus, char ** sztutNextLineIn);
EXTERN int tutGetNextLineFromBuffer( TUTSTATUSDATA *lpTStatus, char ** sztutNextLineIn);
EXTERN int tutNumCharInInputBuffer(TUTSTATUSDATA *lpTStatus);
EXTERN void tutFlushTypeaheadBuffer(TUTSTATUSDATA *lpTStatus);
EXTERN int tutSwitchToTTY( TUTSTATUSDATA *lpTStatus);
EXTERN int tutFetchDataType (int iIndex);
EXTERN float tutFetchDatax2 (int iIndex);
EXTERN char * tutFetchTextData (int iIndex);
EXTERN int tutFetchNumItems ();
EXTERN bool TutStoreRecData (int iNewData, int * Data,int * iNumData, int iSizeData);
EXTERN int FindNextReturn( char * srReturnTerminatedString );
EXTERN int strFindLastNonOccuranceC ( const char *  buf, const char * szTestStr );
EXTERN int strReplaceAllOccurancesInStringC(const char * szString,const char * szOldChar,const char * szNewChar,char * szResult);
EXTERN int strCopySubstringC(const char * szString,int iSubstringStart,int iSubstringEnd,char * szResult);
EXTERN int FixGIDName(char * buf, char * szGID, int iMaxGID);
EXTERN int StripSingleQuotes(char * szGID);
EXTERN int tutCompressAndIndexTutdb();
EXTERN int TutOpenFiles2 ( int iFormat, FILE **fDBFile, FILE **fSeqFile, FILE **fDirectFile , FILE **fIndexFile);
EXTERN int TutCloseFiles2 ( int iFormat, FILE *fDBFile, FILE *fSeqFile, FILE *fDirectFile , FILE *fIndexFile);
EXTERN int iFirstChar(char * buf);
EXTERN int initMemFileSystem(int iMemFileSize, int iMemDirSize);
EXTERN int AddMemFile(Byte * lpMemfile, int iFileSize, char * szFileName, int iOption);
EXTERN int CloseMemFileSystem();
EXTERN int CreateMemFileSource();
EXTERN int LoadMemDir(Byte ** lpMemDir);
EXTERN int UnloadMemDir(Byte ** lpMemDir);
EXTERN int LoadMemFile(Byte ** lpMemDir, const char * szFileName, Byte ** lpMemFile, short unsigned int * FileSize);
EXTERN int tutDisplayTextPromptx ( int iDisplayPointID , int iNumArg );
EXTERN int tutDisplayTextPrompt ( int iDisplayPointID , int NumReplacements , ...  );
EXTERN int tutConvertString (char * szString, int iStringSize);
EXTERN float tutFetchDatax2 (int iIndex);
EXTERN int tutFetchDataType (int iIndex);
EXTERN int tutDisplayPrompt (int iDisplayPointID, int NumReplacements, ...);
EXTERN int tutRecursiveInputInt (int iDispPtNameLong, int iDispPtNameShort, int* Data, int iSizeData, int* iNumData, int iNumArg, ...);
EXTERN int initMemFileSystem(int iMemFileSize, int iMemDirSize);
EXTERN int AddMemFile(Byte * lpMemfile, int iFileSize, char * szFileName, int iOption);
EXTERN int CloseMemFileSystem();
EXTERN int CreateMemFileSource();
EXTERN int ExtractFile ( unsigned int iResID , const char* sRestype , char* Filename, int NumReplacements , ... );
EXTERN int tutIndexTutdb(Byte ** lpMemFile, int * iFileSize, char * szIncludeFileName );
EXTERN int LoadMemFile2(const char * szFileName, Byte ** lpMemFile, short unsigned int * FileSize);
EXTERN void SetParseFmt(int fmt, int numdelim, ...);


extern bool bIsStdinTty;	// defined in tutIsTty
//extern void SetParseFmt(int fmt, int numdelim, ...);
extern int * ParseLineRCfmtItem;
extern char ** ParseLineRCItems;
extern int ParseLineRCNumItems;
//extern char ** SetParseFmtDelims;
//extern int SetParseFmtNumDelims;
//extern int SetParseFmtfmt;
extern int LengthOfInputLine;
extern bool bOutputDP;	// defined in tutIsTty
EXTERN bool btutFirstPromtCall INITBOOLTRUE;
EXTERN TUTSTATUSDATA *lpTStatus INITNULL;
EXTERN char * sztutNextLine INITNULL;

EXTERN Byte * lpTutMemFile INITNULL;  // defined in tutio.c
EXTERN bool bMemDirLoaded INITBOOLFALSE;
EXTERN Byte *uncomprMemFileSystem INITNULL;	// defined in tutio.c


EXTERN uLong uncomprMemFileSystemLen INITBUFFERSIZE;
EXTERN uLong uMaxDirSize INITDIRSIZE;
EXTERN DIRREC2 *Directory;
EXTERN char **FileNameList INITNULL;
EXTERN int iFileNumber INITIZERO;
EXTERN long unsigned int TotalMemFileSystemSize INITIZERO;


#define DISPLAYPROMPT	1
#define DISPLAYTEXTPROMPT	2
#define DISPLAYRECURSIVEPROMPT	3


// define tutlib3 debug flags

#ifndef FROM_DEBUGLIB2
#include "debug.h"
#endif

#ifndef __MYFILE__
#ifdef WINDOZE
#define __MYFILE__  (strrchr ( __FILE__ , '\\' )+1)
#else	// WINDOZE
#define __MYFILE__  (strrchr ( __FILE__ , '/' )+1)
#endif	// WINDOZE
#endif	// __MYFILE__


#endif /* TUTLIB_H_ */
