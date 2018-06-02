#ifdef WINDOZE
#pragma message( "Compiling " __FILE__ " on " __DATE__ " at " __TIME__ )
#pragma message( "File last modified on " __TIMESTAMP__ )
#pragma message( "  ")
#pragma title( "My Secret Box version 2.0" )
#pragma subtitle( "Copyright (c) 2003 - 2015, Nehemiah Ministries, Inc." )
#pragma comment( compiler )
#pragma comment( user, "File: " __FILE__ ". Compiled on " __DATE__ " at " __TIME__ ".  Last modified on " __TIMESTAMP__ )
#endif

#ifdef WINDOZE
#define _WIN32_WINNT 0x0500
//#define WINVER	0x0502
//#define _WIN32_WINNT	0x0502
#include <windows.h>
#include <Wincon.h>
#include <io.h>
#include <stdio.h>
#include <fcntl.h>
#include <iostream>
#include <fstream>
//#pragma comment(lib, "User32.lib")
//#include "passwin.h"
//#include "globals.h"

#ifndef _USE_OLD_IOSTREAMS
using namespace std;
#endif

bool bStdioRedirectedToConsole = false;

	HWND hConWindowHandle = NULL;
	HANDLE hTutConsoleBuffer = NULL;
	float fConWindowRowScale = 0.0;
	float fConWindowColumnScale = 0.0;


int RedirectIOToConsole()
{
	int hConHandle;
	long lStdHandle;	
	CONSOLE_SCREEN_BUFFER_INFO coninfo;
	FILE *fp;
	BOOL bRet;

// allocate a console for this app

//AttachConsole(ATTACH_PARENT_PROCESS);
//bRet = AttachConsole((DWORD)(-1));
#ifdef DEBUGREDIRECT
		WinFprintf(fp9, "calling AllocConsole\n");
#endif	// DEBUGREDIRECT

	bRet = AllocConsole();
#ifdef DEBUGREDIRECT
		WinFprintf(fp9, "AllocConsole returned 0x%x\n",bRet);
#endif	// DEBUGREDIRECT
	if ( !bRet ) {
		DWORD iReterr;
		iReterr = GetLastError();
#ifdef DEBUGREDIRECT
		WinFprintf(fp9, "GetLastError returned 0x%x\n",iReterr);
#endif	// DEBUGREDIRECT
//		return((int)iReterr);
	}

#ifdef DEBUGREDIRECT
		WinFprintf(fp9, "calling CreateConsoleScreenBuffer\n");
#endif	// DEBUGREDIRECT

	hTutConsoleBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,  // access flag
		0,      // buffer share mode   can be zero | FILE_SHARE_READ | FILE_SHARE_WRITE
		NULL , // pointer to security attributes  LPSECURITY_ATTRIBUTES *lpSecurityAttributes
		CONSOLE_TEXTMODE_BUFFER,          // type of buffer to create   The only currently supported screen buffer type is CONSOLE_TEXTMODE_BUFFER.
		NULL   // reserved   LPVOID lpScreenBufferData
	);
	if ( hTutConsoleBuffer == INVALID_HANDLE_VALUE ) {
		MessageBox(NULL,"CreateConsoleScreenBuffer failed in RedirectIOToConsole\nWindows could not create a console window","Text mode failed",MB_OK);
#ifdef DEBUGREDIRECT
		WinFprintf(fp9, "CreateConsoleScreenBuffer failed in RedirectIOToConsole, GetLastError() = 0x%x\n", GetLastError());
#endif	// DEBUGREDIRECT
		return(2);
	}
#ifdef DEBUGREDIRECT
	WinFprintf(fp9, "hTutConsoleBuffer = 0x%x\n", hTutConsoleBuffer);
#endif	// DEBUGREDIRECT

	hConWindowHandle = GetConsoleWindow();
	if ( hConWindowHandle == NULL ) {
		MessageBox(NULL,"GetConsoleWindow failed in RedirectIOToConsole\nWindows could not create a console window","Text mode failed",MB_OK);
#ifdef DEBUGREDIRECT
		WinFprintf(fp9, "GetConsoleWindow failed in RedirectIOToConsole\n");
#endif	// DEBUGREDIRECT
		return(3);
	}
#ifdef DEBUGREDIRECT
	WinFprintf(fp9, "hConWindowHandle = 0x%x\n", hConWindowHandle);
#endif	// DEBUGREDIRECT




// redirect unbuffered STDOUT to the console
	lStdHandle = (long)GetStdHandle(STD_OUTPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stdout = *fp;
	setvbuf( stdout, NULL, _IONBF, 0 );

// redirect unbuffered STDIN to the console
	lStdHandle = (long)GetStdHandle(STD_INPUT_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "r" );
	*stdin = *fp;
	setvbuf( stdin, NULL, _IONBF, 0 );

	bStdioRedirectedToConsole = TRUE;

// redirect unbuffered STDERR to the console
	lStdHandle = (long)GetStdHandle(STD_ERROR_HANDLE);
	hConHandle = _open_osfhandle(lStdHandle, _O_TEXT);
	fp = _fdopen( hConHandle, "w" );
	*stderr = *fp;
	setvbuf( stderr, NULL, _IONBF, 0 );

// make cout, wcout, cin, wcin, wcerr, cerr, wclog and clog
// point to console as well

	ios::sync_with_stdio();


	// set the screen buffer to be big enough to let us scroll text

	GetConsoleScreenBufferInfo(hTutConsoleBuffer,&coninfo);
    // Set the screen buffer size to 150 columns by 250 lines. 
#ifdef DEBUGREDIRECT
	WinFprintf(fp9, "GetConsoleScreenBufferInfo reports rows,columns = %d, %d\n", coninfo.srWindow.Bottom - coninfo.srWindow.Top  + 1,coninfo.srWindow.Right  - coninfo.srWindow.Left + 1);
#endif	// DEBUGREDIRECT


	COORD dwScreenBufferSize;
	dwScreenBufferSize.X = 150;
	dwScreenBufferSize.Y = 250;

    if (! SetConsoleScreenBufferSize((HANDLE)lStdHandle, dwScreenBufferSize) )
    {
        MessageBox(NULL,"SetConsoleScreenBufferSize failed in RedirectIOToConsole","Text mode failed",MB_OK); 
        return 1;
    }

	RECT r;
    GetClientRect(hConWindowHandle, &r); //gets the console's current dimensions 
#ifdef DEBUGREDIRECT
	WinFprintf(fp9, "GetClientRect reports left,right,top,bottom = %d, %d, %d, %d\n",r.left,r.right,r.top,r.bottom);
#endif	// DEBUGREDIRECT

	fConWindowColumnScale = ((float)(coninfo.srWindow.Right  - coninfo.srWindow.Left + 1))/((float)r.right);
	fConWindowRowScale    = ((float)(coninfo.srWindow.Bottom - coninfo.srWindow.Top  + 1))/((float)r.bottom);
#ifdef DEBUGREDIRECT
	WinFprintf(fp9, "fConWindowColumnScale,fConWindowRowScale = %f, %f\n", fConWindowColumnScale,fConWindowRowScale);
#endif	// DEBUGREDIRECT


	return(0);
}

#else	// WINDOZE

int RedirectIOToConsole()
{
	return(0);
}

#endif	// WINDOZE
