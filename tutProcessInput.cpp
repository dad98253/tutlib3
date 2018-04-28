/*
 * tutProcessInput.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"

extern int MsgBox(const char * message, ...);
extern int ParseLineRC(char* ctemp);

bool tutProcessInput( TUTSTATUSDATA *lpTStatus, DPINFO *DPInfo, char * szDefaultInput , int iCallerID ) {
	// get next line of input
	int iRet;
	if  ( (iRet = tutGetNextLineInput( lpTStatus, &sztutNextLine )) < 0 ) {
		return false;
	}
//	if ( iRet == 0 ) {
//		if ( sztutNextLine != NULL ) free(sztutNextLine);
//		sztutNextLine = (char *)malloc(strlen(szDefaultInput)+1);
//		if ( sztutNextLine == NULL ) {
//			MsgBox("malloc failed for sztutNextLine at line %i in tutProcessInput",__LINE__);
//			return(false);
//		}
//	}
#ifdef DEBUG
	if (debugflag) dfprintf(fp9,"sztutNextLine = %s\n",sztutNextLine);
#endif
	//	struct DPINFO {
	//		bool bVariableNumberOfFields = false;		// true => V, false => F
	//		int iNumFieldsAllowed = 0;
	//		int iTypeOfData = 0;		// 1 => char *, 2 => int , 3 => string, 4 => any
	//		int iStartRange = 0;
	//		int iEndRange = 0;
	//		int iFieldNumber = 0;	// number of fields found on the *WRET card
	//		bool bTo = false;		// true => TO, false => OR or none
	//	 };

	/// determine if the input is correct

	// parse the line into a set of delimited string data (character "words" seprated by comma or spaces
	LengthOfInputLine = strlen(sztutNextLine);
	// check for default input
	if ( LengthOfInputLine == 0 && szDefaultInput != NULL ) {
		if ( sztutNextLine != NULL ) {
			free(sztutNextLine);
			sztutNextLine = NULL;
		}
		if ( ( sztutNextLine = (char *)calloc(1,strlen(szDefaultInput)+1)) == NULL ) {
			MsgBox("calloc failed for sztutNextLine at line %i in tutProcessInput",__LINE__);
			return(false);
		}
		strcpy(sztutNextLine,szDefaultInput);
		LengthOfInputLine = strlen(sztutNextLine);
#ifdef DEBUG
		if (debugflag) dfprintf(fp9,"using default input at line %i in tutProcessInput\n",__LINE__);
#endif
	}
	int nParse = 0;
#ifdef DEBUG
	if (debugflag) dfprintf(fp9,"in tutProcessInput sztutNextLine=%s\n",sztutNextLine);
#endif
#ifdef DEBUG
	if (debugflag) dfprintf(fp9,"init nParse = %i\n",nParse);
#endif
	if ( iCallerID != DISPLAYTEXTPROMPT ) {
		if ((nParse=ParseLineRC(sztutNextLine))<0) {
#ifdef DEBUG
	if (debugflag) dfprintf(fp9,"nParse returned from ParseLineRC = %i\n",nParse);
#endif
			return (false);
		}
#ifdef DEBUG
		if (debugflag) {
			dfprintf(fp9,"found %i items\n",nParse);
			int i;
			const char *szformat[] = {"unknown0","char","int","string","unknown4","unknown5","unknown6","unknown7"};
			for (i=0;i<nParse;i++){
				dfprintf(fp9,"item[%i] (fmt %i (%s)) = \"%s\"\n",i,ParseLineRCfmtItem[i],szformat[ParseLineRCfmtItem[i]],ParseLineRCItems[i]);
			}
			dfprintf(fp9,"%i fields(fmt) = ",nParse);
			for (i=0;i<nParse;i++){
				dfprintf(fp9,"%s(%i); ",ParseLineRCItems[i],ParseLineRCfmtItem[i]);
			}
			dfprintf(fp9,"\n");
		}
#endif
		char *szTypeField;
		szTypeField = (char *)"alphanumeric word(s)";
		if ( DPInfo->iTypeOfData == 2 ) szTypeField = (char *)"integer number(s)";
		if ( DPInfo->iTypeOfData == 3 ) szTypeField = (char *)"string(s)";
		if ( DPInfo->iTypeOfData == 4 ) szTypeField = (char *)"any(s)";
		// check for correct number of fields
		if ( !DPInfo->bVariableNumberOfFields && nParse != DPInfo->iNumFieldsAllowed ) {
			MsgBox("Input error...\n Enter exactly %i %s.",DPInfo->iNumFieldsAllowed,szTypeField);
			if ( sztutNextLine != NULL ) {
				free(sztutNextLine);
				sztutNextLine = NULL;
			}
			return(false);
		}
		if ( DPInfo->bVariableNumberOfFields &&  nParse > DPInfo->iNumFieldsAllowed ) {
			MsgBox("Input error...Too many fields of data input.\n Enter %i or fewer %s.",DPInfo->iNumFieldsAllowed,szTypeField);
			if ( sztutNextLine != NULL ) {
				free(sztutNextLine);
				sztutNextLine = NULL;
			}
			return(false);
		}
		// check type of data input
		for (int iField=0;iField<nParse;iField++){
			if ( DPInfo->iTypeOfData != 4 && ParseLineRCfmtItem[iField] != DPInfo->iTypeOfData ) {
				char * szTypeFieldInput;
				szTypeFieldInput = (char *)"alphanumeric";
				if ( ParseLineRCfmtItem[iField] == 2 ) szTypeFieldInput = (char *)"integer";
				if ( ParseLineRCfmtItem[iField] == 3 ) szTypeFieldInput = (char *)"string";
				MsgBox("Input error...Field number %i of the data that was input is of type %s.\n You must enter %s here.",iField+1,szTypeFieldInput,szTypeField);
				if ( sztutNextLine != NULL ) {
					free(sztutNextLine);
					sztutNextLine = NULL;
				}
				return(false);
			}
		}
		// check the data range (skip for recursive input)
		if ( DPInfo->iFieldNumber > 3 && DPInfo->iTypeOfData == 2 && iCallerID != DISPLAYRECURSIVEPROMPT ) {
			for (int iField=0;iField<nParse;iField++){
				int iIntValue;
				sscanf(ParseLineRCItems[iField],"%i",&iIntValue);
				if ( iIntValue < DPInfo->iStartRange || iIntValue > DPInfo->iEndRange ) {
					char * szToOr;
					char * szFromEither;
					szToOr = (char *)"or";
					szFromEither = (char *)"of either";
					if ( (DPInfo->iEndRange - DPInfo->iStartRange) > 1 ) {
						szToOr = (char *)"to";
						szFromEither = (char *)"from";
					}
					MsgBox("Input error...Field number %i of the data that was input is %i.\n Only integers %s %i %s %i are allowed here.",iField+1,iIntValue,szFromEither,DPInfo->iStartRange,szToOr,DPInfo->iEndRange);
					if ( sztutNextLine != NULL ) {
						free(sztutNextLine);
						sztutNextLine = NULL;
					}
					return(false);
				}
			}
		}
		// all done with checks, for parsed data, we no longer need a copy of sztutNextLine
		if ( sztutNextLine != NULL ) {
			free(sztutNextLine);
			sztutNextLine = NULL;
		}
	}  // end of if,then on iCallerID... note that for non-parsed input, sztutNextLine must be freed! a sztutNextLine string will remain at program termination unless tutor is closed properly
	return(true);
}



