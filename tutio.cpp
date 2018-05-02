/*
 * tutio.cpp
 *
 *  Created on: Feb 10, 2016
 *      Author: dad
 */

#ifdef WINDOZE
#include <windows.h>
#include <io.h>
#include <direct.h>
#define STDIN_FILENO	0
#else
#include <unistd.h>
#endif
#include <time.h>
#include <sys/stat.h>
#define TUTLIBMAIN
#include "tutlib.h"
#include "tuterrors.h"
#include "MemSource.h"

extern void SetParseFmt(int fmt, int numdelim, ...);
extern int MsgBox(const char * message, ...);

#ifdef TESTTUTX2
#define DATADIM		10000
extern int DataFromTut[DATADIM];
#endif  //  TESTTUTX2

// temporary stuff to keep uliub working for now...
#define IUIN    5
#define IUOUT   6
#define ITUTIN  9
#define ITUTOT  10
#define ITUTDB  7
//extern "C" void UIOF( int KIN , int KOUT , int KOUTXX , int KTUTXX , int KTUTDB );
//extern "C" void UREADBFF(  );
#ifdef WINDOZE
#ifndef TEXTMODEONLY
extern BOOL bStdioRedirectedToConsole;
#endif  // TEXTMODEONLY
#endif  // WINDOZE

#ifdef DEBUG
extern void DumpSymbols(char * where);
#endif

// If GENMEMFILE is defined, the program has the capability to generate a new memory file system. However, the global
// boolian bGenMemFileSource must also be true for the generation to take place. Note that GENMEMFILE should
// NOT be defined when compiling a release version of the program!
#ifdef GENMEMFILE
int GenMemFile (short unsigned int * FileSize,int * iTutMemFileSize,bool bGenMemFileSource);
#endif

int tutio (char *szInputFile, char *szOutputFile, char * szTutlogFilenameParm , char * szTutDebugFilenameParm , char * szTutdbFilenameParm
		, char * szTutdbDAFilenameParm, char * szTutdbINDFilenameParm) {
//    char P1[2], P2[2], P3[2], P4[2];
//    char V1[2], V3[2], V4[2];
//    char V2[NCHARS+1];
//    int ncar = 12;
	
    short unsigned int FileSize;
#if defined(GENMEMFILE) || defined(DEBUG)
	int iTutMemFileSize = 0;
#endif	// GENMEMFILE || DEBUG
#ifdef DEBUG
		DumpSymbols((char*)"tutio");
#endif

#ifdef GENMEMFILE
	bool bGenMemFileSource = true;
#else
	bool bGenMemFileSource = false;
#endif

#ifdef DOMYTUTIO

    if (szTutdbFilenameParm == NULL) {
			return(NOTUTORFILE);
		} else {
			if ( (fTutdbFilename = fopen(szTutdbFilenameParm,"r")) == NULL) return(TUTORFILEOPENFAIL);
		}

    if (szTutdbDAFilenameParm == NULL) {
			if ( (fTutdbDAFilename = fopen("tut39.daf","r")) == NULL ) return(TUTORDAFILEOPENFAIL);
		} else {
			if ( (fTutdbDAFilename = fopen(szTutdbDAFilenameParm,"r")) == NULL ) return(TUTORDAFILEOPENFAIL);
		}

    if (szTutdbINDFilenameParm == NULL) {
			if ( (fTutDebugFilename = fopen("tut39.seq","r")) == NULL ) return(TUTORINDFILEOPENFAIL);
		} else {
			if ( (fTutDebugFilename = fopen(szTutdbINDFilenameParm,"r")) == NULL ) return(TUTORINDFILEOPENFAIL);
		}

    if (szInputFile == NULL) {
        	fInputFile = stdin;
        } else {
        	if ( (fInputFile = fopen(szInputFile,"r")) == NULL ) return(TUTINFILEOPENFAIL);
        }

    if (szOutputFile == NULL) {
			fOutputFile = stdout;
		} else {
			if ( (fOutputFile = fopen(szOutputFile,"w")) == NULL ) return(TUTOUTFILEOPENFAIL);
		}

    if (szTutlogFilenameParm == NULL) {
			fTutlogFilename = NULL;
		} else {
			if ( (fTutlogFilename = fopen(szTutlogFilenameParm,"w")) == NULL ) return(TUTLOGFILEOPENFAIL);
		}

    if (szTutDebugFilenameParm == NULL) {
			fTutDebugFilename = NULL;
		} else {
			if ( (fTutDebugFilename = fopen(szTutDebugFilenameParm,"w")) == NULL ) return(TUTDEBUGFILEOPENFAIL);
		}

#else    // DOMYTUTIO



	// check if stdin is tty...
#ifdef WINDOZE
#ifndef TEXTMODEONLY
	if ( bStdioRedirectedToConsole ) { bOutputDP = bIsStdinTty = true; } else { bOutputDP = bIsStdinTty = false; }
#else  //  TEXTMODEONLY
	if ( isatty(STDIN_FILENO) != 0 ) { bOutputDP = bIsStdinTty = true; } else { bOutputDP = bIsStdinTty = false; }
#endif  //  TEXTMODEONLY
#else  //  WINDOZE
    if ( isatty(STDIN_FILENO) != 0 ) {
//	if ( isatty(fileno(stdin)) != 0 ) {
		bOutputDP = bIsStdinTty = true; }
	else {
		bOutputDP = bIsStdinTty = false; }
#endif  //  WINDOZE
#ifdef DEBUG
	if (debugflag) {
		 if (bIsStdinTty) {
				dfprintf(fp9,"stdin is a tty\n");
		 } else {
				dfprintf(fp9,"stdin is not a tty\n");
		 }
	}
#endif  // DEBUG

	// initialize the parse parameters for ParseLineRC
	SetParseFmt(0,0);
	SetParseFmt(1,2," ",",");

#ifdef GENMEMFILE
	if ( bGenMemFileSource ) {
		int iGenRet;
		if ( (iGenRet = GenMemFile( &FileSize, &iTutMemFileSize, bGenMemFileSource)) ) return (iGenRet);
	}
#endif  // GENMEMFILE










// test tutor file
#ifdef DEBUG
		dfprintf(fp9, "memory file zlib version = \"%s\"\n",myzlibVersion);
#endif
	if ( !bGenMemFileSource ) uncomprMemFileSystem = ucMemFileSystemData;		// if we didn't just finished generating the
																				// memory file system, use the old data that
																				// was previously compile in
	if ( LoadMemFile2("tutorDB.txt", &lpTutMemFile, &FileSize ) != 0 ) {		// extract tutor the file
#ifdef DEBUG
		dfprintf(fp9, "LoadMemFile2 failed in \"tutorDB.txt\"\n");
#endif

		return(-15);
	}

#ifdef DEBUG
		dfprintf(fp9, "LoadMemFile2 extracted \"%s\", size returned = %i, size should be = %i\n","tutorDB.txt",FileSize,iTutMemFileSize);
#endif

#ifdef DEBUG
// print some debug infor about the tty device
		struct stat sb;
#ifdef WINDOZE
		char * sztty=(char *)"passwin.exe";
#else	// WINDOZE
		char * sztty=(char *)"/dev/tty";
#endif	// WINDOZE
		if (stat(sztty, &sb) == -1) {
			    fprintf(stderr,"stat call in GetFileTime failed for %s\n",sztty);
		        perror("stat");
		        return(true);
		    }

		if (debugflag) {
			dfprintf(fp9,"\33[1;35mGetFileTime\33[0m stat of \'%s\':\nFile type:                ",sztty);

			switch (sb.st_mode & S_IFMT) {
				case S_IFREG:  dfprintf(fp9,"regular file\n");            break;
				case S_IFDIR:  dfprintf(fp9,"directory\n");               break;
#ifndef WINDOZE
				case S_IFBLK:  dfprintf(fp9,"block device\n");            break;
				case S_IFCHR:  dfprintf(fp9,"character device\n");        break;
				case S_IFIFO:  dfprintf(fp9,"FIFO/pipe\n");               break;
				case S_IFLNK:  dfprintf(fp9,"symlink\n");                 break;
				case S_IFSOCK: dfprintf(fp9,"socket\n");                  break;
#endif		// WINDOZE
				default:       dfprintf(fp9,"unknown\n");                 break;
			}
			dfprintf(fp9,"I-node number:            %ld\n", (long) sb.st_ino);
			dfprintf(fp9,"Mode:                     %lo (octal)\n",(unsigned long) sb.st_mode);
			dfprintf(fp9,"Link count:               %ld\n", (long) sb.st_nlink);
			dfprintf(fp9,"Ownership:                UID=%ld   GID=%ld\n",(long) sb.st_uid, (long) sb.st_gid);
#ifdef WINDOZE
			dfprintf(fp9,"File size:                %lld bytes\n",(__int64) sb.st_size);
#else		// WINDOZE
			dfprintf(fp9,"Preferred I/O block size: %ld bytes\n",(long) sb.st_blksize);
			dfprintf(fp9,"File size:                %lld bytes\n",(long long) sb.st_size);
			dfprintf(fp9,"Blocks allocated:         %lld\n",(long long) sb.st_blocks);
#endif		// WINDOZE
			dfprintf(fp9,"Last status change:       %s", ctime(&sb.st_ctime));
			dfprintf(fp9,"Last file access:         %s", ctime(&sb.st_atime));
			dfprintf(fp9,"Last file modification:   %s", ctime(&sb.st_mtime));
		}
#endif		// DEBUG
//	test tutor
#ifdef TESTTUTX2

	EchoOff();
	tutDisplayTextPrompt ( ENTER_MASTER_PASSWORD , 1 , "CHAR" , "STR" );   //  SET_MIN_BACKUP_AGE       MAIN_MENU
	EchoOn();
	char szpassword[151];
	tutConvertString (szpassword, 150);
#ifdef DEBUG
		dfprintf(fp9, "tutConvertStringx2 returned \"%s\"\n",szpassword);
#endif		// DEBUG

	tutDisplayPrompt ( NO_DATABASE , 0 );
	int imyvalue;
	imyvalue = (int)tutFetchDatax2(1);
#ifdef DEBUG
		dfprintf(fp9, "tutFetchDatax2 returned \"%i\"\n",imyvalue);
#endif		// DEBUG

	tutDisplayPrompt ( SET_MIN_BACKUP_AGE , 1 , "MINBACKUPAGE" , "20");
	imyvalue = (int)tutFetchDatax2(1);
#ifdef DEBUG
		dfprintf(fp9, "tutFetchDatax2 returned \"%i\"\n",imyvalue);
#endif		// DEBUG

	tutDisplayPrompt ( MAIN_MENU , 4 , "COLOR1" , "\33[1;31m" , "COLOR2" , "\33[0m" , "BANNER" , "My VERY Kool Banner!" , "COLOR3" , "\33[1;32m");
// old line //	tutDisplayTextPromptx2 ( MAIN_MENU , 3 , "COLOR1" , "1New collor 1" , "COLOR3" , "3my new collor 3" , "BANNER" , "My VERY Kool Banner!" );
	imyvalue = (int)tutFetchDatax2(1);
#ifdef DEBUG
		dfprintf(fp9, "tutFetchDatax2 returned \"%i\"\n",imyvalue);
#endif		// DEBUG

//	const char szDispPtNameLong[] = {"ENTER RANGE1"};
//	const char szDispPtNameShort[] = {"ENTER RANGE1B"};
	int iNumData;
	char szSizeData[20];
	sprintf(szSizeData,"%i",DATADIM);
	tutRecursiveInputInt (ENTER_RANGE1, ENTER_RANGE1B, DataFromTut, DATADIM, &iNumData, 4, "NUMSIZ", szSizeData, "INT" , "ANY" , "ITEMS", "Index Numbers", "OPERATION", "List");
#ifdef DEBUG
		dfprintf(fp9, "tutRecursiveInputIntx2 returned \"%i\" values\nvaluse are :",iNumData);
		for (int ixxx=0;ixxx<iNumData;ixxx++) dfprintf(fp9, " %i,",DataFromTut[ixxx]);
		dfprintf(fp9, "\n");
#endif		// DEBUG
#endif  // TESTTUTX2

//	free(lpTutMemFile);		// free the tutor file
//	lpTutMemFile = NULL;

	if ( bGenMemFileSource && ( uncomprMemFileSystem != NULL ) ) free(uncomprMemFileSystem);		// free the temproary mem file system

#endif   // (end else) DOMYTUTIO

	return(0);
}







#ifdef GENMEMFILE

int GenMemFile (short unsigned int *FileSize,int *iTutMemFileSize,bool bGenMemFileSource) {

	Byte * lpMemDir;
	int NumCompressedRecords = 0;
// read the ascii "fort.1" tutor database file, process it and create the tutIndex.h and tutSource.61 files
    if ( ( NumCompressedRecords = tutCompressAndIndexTutdb() ) < 0) {
    	printf("error in tutCompressAndIndexTutdb\n");
    }
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"tutCompressAndIndexTutdb compressed %u records\n",NumCompressedRecords);
#endif

//    Byte * lpTutMemFile;
//    if ( tutLoadTutMemDB(&lpTutMemFile) < 0) {
//    	printf("error in tutLoadTutdb\n");
//    }

// load the memory file system directory
// NOTE: this will be the file system presently saved in tutSource.h and tutIndex.h files located in
// your build directory, NOT the files created by tutCompressAndIndexTutdb above!
    if ( LoadMemDir(&lpMemDir) < 0) {
    	printf("error in LoadMemDir\n");
    }
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"LoadMemDir done\n");
#endif
// load the tutorial database file from the memory file system
    if ( LoadMemFile(&lpMemDir, "tutorDB.txt", &lpTutMemFile, FileSize) < 0) {
    	printf("error in LoadMemFile\n");
    }
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"LoadMemFile done\n");
#endif
//	tutDisplayTextPromptx ( ENTER_MASTER_PASSWORD , 0 );   //  SET_MIN_BACKUP_AGE       MAIN_MENU
//	tutDisplayTextPromptx ( NO_DATABASE , 0 );
//	tutDisplayTextPromptx ( SET_MIN_BACKUP_AGE , 0 );
//	tutDisplayTextPromptx ( MAIN_MENU , 0 );

// unload the memeory file system directory
// NOTE : this frees up the entire uncompressed file system and thus, the lpTutMemFile pointer
// found by LoadMemFile above will no longer be valid!
    if ( UnloadMemDir(&lpMemDir) < 0) {
    	printf("error in UnloadMemDir\n");
    }
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"UnloadMemDir done\n");
#endif




    // test mem file system
	// initialize the system for building
    int iDirSize = initMemFileSystem(30000, 10);
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"initMemFileSystem returned dir size = %u\n",iDirSize);
#endif


// load the SettingsResetREADME.txt file into the memfile system
// ... these are the help instructions for hacking the passwin database
	FILE * fSettingsResetREADME;
	if ((fSettingsResetREADME = fopen("SettingsResetREADME.txt", "r")) == NULL) {
		MsgBox("unable to open SettingsResetREADME.txt file for read");
		return(-7);
	}
	Byte * ucSettingsResetREADME = NULL;
#define RESETREADMEBUFFSIZE		30000
	ucSettingsResetREADME = (Byte *)malloc(RESETREADMEBUFFSIZE);		// get some scratch memory
	if ( ucSettingsResetREADME == NULL ) {
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"malloc failed for ucSettingsResetREADME in tutio\n",iDirSize);
#endif
		return(-8);
	}
	int iSettingsResetREADMESize =  fread(ucSettingsResetREADME, 1, RESETREADMEBUFFSIZE, fSettingsResetREADME ); // load the whole file into scratch memory
	fclose(fSettingsResetREADME);
	if ( AddMemFile(ucSettingsResetREADME, iSettingsResetREADMESize, (char *)"SettingsResetREADME.txt", 1) < 0 ) {  // add it to the mem file system
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"AddMemFile failed for SettingsResetREADME.txt in tutio\n");
#endif
		free(ucSettingsResetREADME);		// free the scratch memory
		return(-9);
	}
	free(ucSettingsResetREADME);		// free the scratch memory
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"SettingsResetREADME.txt added to mem file system\n");
#endif


// load the tutor database file into the memfile system
	lpTutMemFile = NULL;
	int iNumerTutRecords;
// create the tokenized and indexed tutor datbase file
	if ( ( iNumerTutRecords = tutIndexTutdb( &lpTutMemFile, iTutMemFileSize, NULL ) ) <=0 ) {
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"tutIndexTutdb failed in tutio, iNumerTutRecords = %i\n",iNumerTutRecords);
#endif
		return(-11);
	}
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"tutIndexTutdb created %i tutor records, mem file size = %i\n",iNumerTutRecords,*iTutMemFileSize);
#endif
	if ( AddMemFile(lpTutMemFile, *iTutMemFileSize, (char *)"tutorDB.txt", 1) < 0 ) {  // add it to the mem file system (compressed)
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"AddMemFile failed for tutorDB.txt in tutio\n");
#endif
		free(lpTutMemFile);		// free the tutor file
		return(-12);
	}
	free(lpTutMemFile);		// free the tutor file
	lpTutMemFile = NULL;
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"tutorDB.txt added to mem file system\n");
#endif


	CloseMemFileSystem();				// close the mem file system (saves directory info...)
#ifdef DEBUG
	if (debugflag ) dfprintf(fp9,"mem file system directory created and saved\n");
#endif


	if( bGenMemFileSource ) {
		CreateMemFileSource();	// produce new source code for the mem file system

#ifdef DEBUG
	if (  debugflag ) dfprintf(fp9,"source code for the mem file system created and saved\n");
#endif
	}

// test the modified extract file routine
/*	if ( ExtractFile ( 1 , "MYFILEX" , (char *)"output.txt" , 4 ,
													  "SETTINGSLOCATION" , "SubstituteString1",
		                                              "folder" , "SubstituteString2",
		                                              "Secret Box" , "SubstituteString3",
		                                              "9e9n9d9o9f9f9i9l9e9" , ""
													  ) ) {
#ifdef DEBUG
		if (debugflag ) dfprintf(fp9,"ExtractFile failed in tutio\n");
#endif
		return (-14);
	}   */

	return (0);
}

#endif   //  GENMEMFILE    ///////////////////////////////////////////////////////////////////////////////
