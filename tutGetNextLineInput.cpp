/*
 * tutGetNextLineInput.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#ifdef WINDOZE
#include <io.h>
#include <windows.h>
#else
#include <unistd.h>
#endif
#include <stdio.h>
#include <string.h>
#include "tutlib.h"

#ifdef WINDOZE
extern HWND hWnd;
#endif
extern int inputl(FILE * fp, void * p, int length);
extern int MsgBox(const char * message, ...);

int tutGetNextLineInput( TUTSTATUSDATA *lpTStatus, char ** sztutNextLineIn ) {
	if (*sztutNextLineIn != NULL) {
		free(*sztutNextLineIn);
		*sztutNextLineIn = NULL;
	}
	if ( tutNumCharInInputBuffer(lpTStatus) != 0 ) return(tutGetNextLineFromBuffer(lpTStatus,sztutNextLineIn));
#define MAXTUTLINESIZE	1000
// int itutInputSource = 0; // 0 => stdin, 1 => /dev/tty, 2 => string, 3 => other
	if ( lpTStatus->szTypeAheadBuffer != NULL ) free(lpTStatus->szTypeAheadBuffer);
	char mybuff[MAXTUTLINESIZE];
	int length = MAXTUTLINESIZE-1;


	int iRet;
	if( (iRet = inputl(lpTStatus->fTutInputStream, mybuff, length)) < 0 ) {
//	if( (iRet = inputl(stdin, mybuff, length)) < 0 ) {
		if ( lpTStatus->itutInputSource == 0 ) {
			MsgBox("End of input file detected. Switching to keyboard input...");
			fclose(lpTStatus->fTutInputStream);
			fclose(lpTStatus->fTutOutputStream);
#ifdef WINDOZE
			if( (lpTStatus->fTutInputStream = stdin) == NULL ) {
#else
			if( (lpTStatus->fTutInputStream = fopen("/dev/tty","r")) == NULL ) {
#endif
				fprintf(stderr,"Unable to open keyboard for input...\n");
				return(-98);
			}
			if ( !isatty(fileno(stdout)) ) {
#ifdef WINDOZE
				if( (lpTStatus->fTutOutputStream = fopen("/dev/tty","a")) == NULL ) {
#else
				if( (lpTStatus->fTutOutputStream = fopen("/dev/tty","a")) == NULL ) {
#endif
					fprintf(stderr,"Unable to open terminal/console for output...\n");
					return(-98);
				}
			}
			lpTStatus->itutInputSource = 1;
			return(-1);		// return -1 to force re-prompt to console
		}
#ifdef WINDOZE
		// on WINDOZE, we need to flush the message buffer to process timer alarms
		// hWnd is the handle of the MessageOnlyWindow
		else {
			MSG msg;
			while (PeekMessage(&msg, hWnd,  WM_TIMER, WM_TIMER, PM_NOREMOVE)) 
			{
				GetMessage(&msg, hWnd, WM_TIMER, WM_TIMER);
				DispatchMessage(&msg);
			}
		}
#endif
		lpTStatus->iPhysicalLineNumber++;
		return(iRet);
	}
	// successful read of a line, now move it to the read ahead buffer (trimming unecessary length)
	if ( (lpTStatus->szTypeAheadBuffer = (char *)calloc(1,iRet+1)) == NULL ) {
		MsgBox("memory allocation failed -- out of memory");
		return(-99);
	}
	strcpy(lpTStatus->szTypeAheadBuffer,mybuff);
	lpTStatus->iNextCharInBuffer = 0;
	lpTStatus->iLastCharInBuffer = iRet;
	// now get the next line form the read ahead buffer
	return(tutGetNextLineFromBuffer(lpTStatus,sztutNextLineIn));

}



