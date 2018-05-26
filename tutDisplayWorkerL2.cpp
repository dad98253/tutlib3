/*
 * tutDisplayWorkerL2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#ifdef WINDOZE
#include <stdarg.h>
#endif
#include <string.h>
#include "tutlib.h"
#include "tuterrors.h"

extern int MsgBox(const char * message, ...);

extern int ReplaceText2( const char * szStringIn , char ** lpsEditedString , int NumReplacements , va_list argptr);

int tutDisplayWorkerL2 ( int iDisplayPointID, int iCallerID, int NumReplacements , va_list ap  ) {
//	va_list ap;
//	va_start(ap, NumReplacements);

// if this is the first tutor prompt, check to see if we are interactive
    if ( btutFirstPromtCall ) {
    	btutFirstPromtCall = false;
    	if ( lpTStatus == NULL ) {
    		if ( (lpTStatus = (TUTSTATUSDATA *)calloc(1,sizeof(TUTSTATUSDATA))) == NULL ) {
    			return(-99);
    		}
    	}
    	lpTStatus->itutInputSource = 0;
#ifdef WINDOZE
    	lpTStatus->fTutInputStream = stdin;
    	lpTStatus->fTutOutputStream = stdout;
#else
    	lpTStatus->fTutInputStream = stdin;
    	lpTStatus->fTutOutputStream = stdout;
#endif
    	lpTStatus->szTypeAheadBuffer = NULL;
    	lpTStatus->iLogicalLineNumber = 0;
    	lpTStatus->iPhysicalLineNumber = 0;
    }
	long unsigned int uRecOffset;

	// find the display point record
	int iRet;
	if( ( iRet = FindTutDPRecord( iDisplayPointID ) ) < 0 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"FindTutDPRecord failed at line %i in tutDisplayWorker, iRet = %i\n",__LINE__,iRet);
#endif
		return (RET_ABORTPROGRAM);
	}
	uRecOffset = (long unsigned int )iRet;
	// replace any substitue strings and/or make a copy of the DP record
    char * lpsEditedResource;
    int iLenResource;
	if ( (iLenResource = ReplaceText2( (const char *)(lpTutMemFile+uRecOffset) , &lpsEditedResource , NumReplacements , ap )) < 0 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"ReplaceText2 failed at line %i in tutDisplayWorker, iRet = %i\n",__LINE__,iLenResource);
#endif
		return(RET_ABORTPROGRAM);
	}
#ifdef DEBUG
//	dfprintf(__LINE__,__FILE__,TRACE,"iDisplayPointID = %i looks like this after string replacement in %s :\n%s",iDisplayPointID,__MYFILE__,lpsEditedResource);
	dfprintf(__LINE__,__FILE__,TRACE,"iDisplayPointID = %i looks like this after string replacement in %s :\n%s",iDisplayPointID,"??",lpsEditedResource);
#endif
	// parse the DP rcord
//	struct DPINFO {
//		bool bVariableNumberOfFields = false;		// true => V, false => F
//		int iNumFieldsAllowed = 0;
//		int iTypeOfData = 0;		// 1 => char *, 2 => int, 3 => string, 4 => any
//		int iStartRange = 0;
//		int iEndRange = 0;
//		int iFieldNumber = 0;	// number of fields found on the *WRET card
//		bool bTo = false;		// true => TO, false => OR or none
//	 };
	DPINFO DPInfo;
	memset(&DPInfo, 0, sizeof(DPINFO));
	char * szDefaultInput = NULL;
	char * szDPIDString = NULL;
	char * szDPText = NULL;
	if ( (iRet = ParseDPRecord ( lpsEditedResource , iLenResource , &DPInfo , &szDefaultInput , &szDPIDString , &szDPText )) !=0 ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"ParseDPRecord failed at line %i in tutDisplayWorker, iRet = %i\n",__LINE__,iRet);
#endif
		return (RET_ABORTPROGRAM);
	}
// prompt until we get valid data (10 times max)
	int iNumerOfTrys = 0;
	bool bValidData = false;
	int iPromptReturn;
	while (!bValidData ) {
		// display the prompt
		iPromptReturn = tutPrompt( lpTStatus, szDPIDString,szDefaultInput,szDPText);
		if ( iPromptReturn < 0 ) return(RET_ABORTPROGRAM);
		// read and check the input
		bValidData = tutProcessInput( lpTStatus, &DPInfo, szDefaultInput, iCallerID);
		iNumerOfTrys++;
		if ( iNumerOfTrys > 9 ) {
			MsgBox("Are we having problesm?\nThere have been too many failed attempts to input valid data.\nThe program will now terminate.");
			return(RET_ABORTPROGRAM);
		}
	}
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"tutDisplayWorker found valid input at line %i, iRet = %i\n",__LINE__,iRet);
#endif
	if ( szDefaultInput != NULL ) free( szDefaultInput );
	if ( szDPIDString != NULL ) free( szDPIDString );
	if ( szDPText != NULL ) free( szDPText );
	free( lpsEditedResource );

	va_end(ap);
	return(RETURN_OK);
}



