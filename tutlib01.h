/*
 * tutlib01.h
 *
 *  Created on: June 10, 2016
 *      Author: dad
 */

#ifndef TUTLIB01_H_
#define TUTLIB01_H_

struct INDEXREC {
	long unsigned int pos;
   short unsigned int wUncompressedsize;
 };

struct INDEXREC2 {
	long unsigned int pos;
   short unsigned int wRecordSize;
 };

struct DIRREC {
	long unsigned int FileOffset;
	long unsigned int NameOffset;
    short unsigned int wFileSize;
    short unsigned int wNamelength;
 };

struct DIRREC2 {
	long unsigned int FileOffset;
	long unsigned int NameOffset;
    short unsigned int wFileSize;
    short unsigned int wUncompressedFileSize;
    short unsigned int wNamelength;
    Byte flags;
 };

struct DPINFO {
	bool bVariableNumberOfFields;		// true => V, false => F
	int iNumFieldsAllowed;
	int iTypeOfData;		// 1 => char *, 2 => int, 3 => string, 4 => any
	int iStartRange;
	int iEndRange;
	int iFieldNumber;	// number of fields found on the *WRET card
	bool bTo;		// true => TO, false => OR or none
 };

#define CHECK_ERR(err, msg) { \
    if (err != Z_OK) { \
        MsgBox("zlib %s error: %d\nthis is a programming error...", msg, err); \
        return(1); \
    } \
}

struct TUTSTATUSDATA {
	int itutInputSource;		// 0 => stdin, 1 => /dev/tty, 2 => string, 3 => other
	FILE *fTutInputStream;
	FILE *fTutOutputStream;
	char *szTypeAheadBuffer;
	int iNextCharInBuffer;
	int iLastCharInBuffer;
	int iLogicalLineNumber;
	int iPhysicalLineNumber;
 };

struct TUTINPUTDATA {
	int *iDataType;		// 0 => empty, 1 => char, 2 => int, 3 => float
	void ** ppField;
	int iNumValues;
 };

#include "tutlib03.h"


#endif /* TUTLIB01_H_ */
