/*
 * tutRecursiveInputIntx2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include "tutlib.h"
#include "tuterrors.h"

extern int MsgBox(const char * message, ...);

int tutRecursiveInputInt (int iDispPtNameLong, int iDispPtNameShort, int* Data, int iSizeData, int* iNumData, int NumReplacements, ...){
// usage:
//   iRet = tutRecursiveInputInt (szDispPtNameLong,szDispPtNameShort,*Data,iSizeData,*iNumData,
//   iNumArg,[szP1,szV1,szP2,szV2,szP3,szV3,]...)
//
// calling parameters:
//     szDispPtNameLong    - Name of the long version of the Display Point (input)
//     szDispPtNameShort   - Name of the short version of the Display Point (input)
//     Data                - Array to receive the returned data (output)
//     iSizeData           - Size of the Data array (input)
//     iNumData            - The number of entries put in the Data array (output)
//                           = -1 => the "Erase" command was entered
//                                   Note: Data array is undefined when "Erase" is entered
//     NumReplacements     - The number of substitution parameter pairs passed in (szP1,szV1,...) (input)
//     szP1,szV1,...       - The parameters and values to substitute
//
	va_list ap;
	va_start(ap, NumReplacements);

	if ( NumReplacements < 0 ) return (BADUDISPPCCALL);
#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"tutRecursiveInputIntx2 called for %i\n",iDispPtNameLong);
#endif
	*iNumData = 0;
// prompt until we get valid data (10 times max)
	int iNumerOfTrys = 0;
	bool bDone = false;
//	int iPromptReturn;
	bool bLongMenu = true;
	int i;
	while (!bDone ) {
		// display the prompt
		int iRet;
		if ( bLongMenu ) {
			va_start(ap, NumReplacements);   // fix ap list (gets broken if passed out ... possible gcc bug??)
			iRet = tutDisplayWorker ( iDispPtNameLong , DISPLAYRECURSIVEPROMPT , NumReplacements, ap );
			bLongMenu = false;
		} else {
			va_start(ap, NumReplacements);   // fix ap list (gets broken if passed out ... possible gcc bug??)
			iRet = tutDisplayWorker ( iDispPtNameShort , DISPLAYRECURSIVEPROMPT , NumReplacements, ap );
		}
		if ( iRet < 0 ) return(RET_ABORTPROGRAM);
		// read and check the input
#ifdef DEBUG
		dfprintf2(__LINE__,__FILE__,TRACE,"tutRecursiveInputIntx2 found %i inputs\n",ParseLineRCNumItems);
		if ( tutFetchNumItems() > 0 ) {
			for (int ijk = 1; ijk < (tutFetchNumItems()+1) ; ijk++) {
				dfprintf2(__LINE__,__FILE__,TRACE,"field %i (type %i) : ",ijk,tutFetchDataType(ijk));
				if ( tutFetchDataType(ijk) < 2 ) {
					dfprintf2(__LINE__,__FILE__,TRACE,"%s\n",tutFetchTextData(ijk));
				} else {
					dfprintf2(__LINE__,__FILE__,TRACE,"%i\n",(int)tutFetchDatax2(ijk));
				}
			}
		}
#endif

	//	if (batch && EOF) return(RET_ABORTPROGRAM);    //  ------------------------------------------------- need to fix this

		if ( tutFetchNumItems() == 0 ) continue;
		if ( tutFetchNumItems() == 1 && tutFetchDataType(1) < 2) {
			char * szTempString = tutFetchTextData(1);
			if (strcmp (szTempString,"DONE") == 0 || strcmp (szTempString,"Done") == 0 || strcmp (szTempString,"done") == 0 || strcmp (szTempString,"D") == 0 || strcmp (szTempString,"d") == 0 ) {
			// Done
				bDone = true;
				if (*iNumData == 0) MsgBox("Warning : your list is empty!");
				continue;
			}
			if (strcmp (szTempString,"ERASE") == 0 || strcmp (szTempString,"erase") == 0 || strcmp (szTempString,"Erase") == 0 ) {
			//	ERASECOMMAND
				if (*iNumData != 0 ) {
					for (int j=0;j<*iNumData;j++) {
						Data[j] = 0;
					}
					MsgBox("%i items erased",*iNumData);
					*iNumData = 0;
				} else {
					MsgBox("The list is already empty");
				}
				iNumerOfTrys = 0;
				bLongMenu = true;
				continue;
			}
		}


		///////////////////////////////////////////////////////////////////////////////////////////  i am here
		//	parse line;
		//	if (parse error) goto error100;

// check for characters other than "-" and for data fields with more than one "-"
		for (i=1;i<=tutFetchNumItems();i++) {
			if ( tutFetchDataType(i) < 2 ) {
//				if ( strcspn(tutFetchTextDatax2(i),"1234567890-") != strlen(tutFetchTextDatax2(i)) ) {
#ifdef DEBUG
				int ifound = strspn(tutFetchTextData(i),"1234567890-");
				int istrlen = strlen(tutFetchTextData(i));
				dfprintf2(__LINE__,__FILE__,TRACE,"in tutRecursiveInputIntx2 at line %i, ifound = %i, istrlen = %i\n",__LINE__,ifound,istrlen);
#endif
				if ( strspn(tutFetchTextData(i),"1234567890-") != strlen(tutFetchTextData(i)) ) {
					MsgBox("the only characters allowed in a data field are 1234567890 and -");
					goto error100;
				}
				if ( strpbrk((const char *)tutFetchTextData(i), "-") != strrchr((const char *)tutFetchTextData(i), '-' ) ) {
					MsgBox("only one \"-\" is allowed in a data field");
					goto error100;
				}
			}
		}

		for (i=1;i<=tutFetchNumItems();i++) {
	// check for possible range of numbers
			if ( tutFetchDataType(i) < 2 ) {
				// parse the string
				int iStringType = 4;
				if ( strpbrk((const char *)tutFetchTextData(i), "-") == tutFetchTextData(i) ) iStringType = 2;
				if ( strpbrk((const char *)tutFetchTextData(i), "-") == (tutFetchTextData(i)+strlen(tutFetchTextData(i))-1) ) iStringType = 3;
				if ( strcmp((const char *)tutFetchTextData(i),"-") == 0 ) iStringType = 1;
				// check for a beginning and end of a range of numbers
				if ( ( i == 1 && iStringType < 3 ) || ( i == tutFetchNumItems() && (iStringType%2) == 1 ) ) {
					MsgBox("error in field number %i. \"-\" cannot be the first or last character on the line. A range of numbers must have a beginning and an end.",i);
					goto error100;
				}
				// a valid range syntax has been found.. find the start and end numbers
				int iStart,iEnd;
				int iErr,iTemp;
				char * szTempString;
				char * szDashLoc;
				switch (iStringType) {
				case 1:  // "-"
					if ( tutFetchDataType(i-1) < 2 || tutFetchDataType(i+1) < 2 ) {
						MsgBox("error in field number %i. The fields on both sides of a \"-\" must be numbers.",i);
						goto error100;
					}
					iStart = (int)tutFetchDatax2(i-1);
					iEnd   = (int)tutFetchDatax2(i+1);
					i++;
					break;

				case 2:  // "-#"
					if ( tutFetchDataType(i-1) < 2 ) {
						MsgBox("error in field number %i. The field to the left of a \"-\" must be a number.",i);
						goto error100;
					}
					iStart = (int)tutFetchDatax2(i-1);
					// parse the number part on the right
					if ( (iErr = sscanf((tutFetchTextData(i)+1),"%i",&iEnd)) != 1 ) {
						MsgBox("error in parsing %s. I'm confused and can\'t figure out what to do!",tutFetchTextData(i));
						goto error100;
					}
					break;

				case 3:  // "#-"
					if ( tutFetchDataType(i+1) < 2 ) {
						MsgBox("error in field number %i. The field to the right of a \"-\" must be a number.",i);
						goto error100;
					}
					iEnd = (int)tutFetchDatax2(i+1);
					// parse the number part on the left ... we will need a scratch location to strip off the trailing "-"
					szTempString = NULL;
					if ( ( szTempString = (char *)malloc( strlen(tutFetchTextData(i))+1 ) ) == NULL ) {
						MsgBox("error in parsing %s. I'm out of memory!",tutFetchTextData(i));
						goto error100;
					}
					strcpy(szTempString,(const char *)tutFetchTextData(i));
					iTemp = strlen(szTempString)-1;
					szTempString[iTemp] = '\000';
					if ( (iErr = sscanf(szTempString,"%i",&iStart)) != 1 ) {
						free(szTempString);
						MsgBox("error in parsing %s. I'm confused and can\'t figure out what to do!",tutFetchTextData(i));
						goto error100;
					}
					free(szTempString);
					i++;
					break;

				case 4:  // "#-#"
					// parse the number part on the left ... we will need a scratch location to strip off the trailing "-"
					szTempString = NULL;
					if ( ( szTempString = (char *)malloc( strlen(tutFetchTextData(i))+1 ) ) == NULL ) {
						MsgBox("error in parsing %s. I'm out of memory!",tutFetchTextData(i));
						goto error100;
					}
					strcpy(szTempString,(const char *)tutFetchTextData(i));
					szDashLoc = (char *)strpbrk((const char *)szTempString, "-");
					*szDashLoc = '\000';
					if ( (iErr = sscanf(szTempString,"%i",&iStart)) != 1 ) {
						MsgBox("error in parsing %s. I'm confused and can\'t figure out what to do!",tutFetchTextData(i));
						free(szTempString);
						goto error100;
					}
					free(szTempString);
					szDashLoc = (char *)strpbrk((const char *)tutFetchTextData(i), "-");
					if ( (iErr = sscanf((szDashLoc+1),"%i",&iEnd)) != 1 ) {
						MsgBox("error in parsing %s. I'm confused and can\'t figure out what to do!",tutFetchTextData(i));
						goto error100;
					}
					break;

				default:  // impossible
					break;

				}  // end of iStringType switch
				// check for a valid range
				if ( iStart > iEnd ) {
					MsgBox("error in range specification (%i-%i). The start must be smaller than the end!",iStart,iEnd);
					goto error100;
				}
				for (int j=iStart;j<iEnd+1;j++) {
					if ( !TutStoreRecData(j,Data,iNumData,iSizeData) ) goto error100;
				}

			}  else { // end of if, then on range field

	//		SINGLENUMBER:
				if ( !TutStoreRecData ((int)tutFetchDatax2(i),Data,iNumData,iSizeData) ) goto error100;
			}        // end of if, else on range field

			iNumerOfTrys = 0;

		}  // end of do loop on i

		continue;

// error in input...
error100:
		tutFlushTypeaheadBuffer(lpTStatus);
		if ( !bIsStdinTty ) {
			// list bad line and line number...
			if ( sztutNextLine != NULL ) {
				MsgBox("error on line number %i :\n %s",lpTStatus->iPhysicalLineNumber,sztutNextLine);
			} else {
				MsgBox("error on line number %i",lpTStatus->iPhysicalLineNumber);
			}
			// if (terminal available) switch to interactive mode
			if ( tutSwitchToTTY(lpTStatus) < 0 ) {
				MsgBox("unable to switch to interactive mode\nThe program will now terminate.");
				return(RET_ABORTPROGRAM);
			}
		} else {
			MsgBox("try again...");
		}

		bLongMenu = true;
		iNumerOfTrys++;
		if ( iNumerOfTrys > 9 ) {
			MsgBox("Are we having problesm?\nThere have been too many failed attempts to input valid data.\nThe program will now terminate.");
			return(RET_ABORTPROGRAM);
		}

	}  // end of while on bDone

#ifdef DEBUG
	dfprintf2(__LINE__,__FILE__,TRACE,"tutRecursiveInputIntx2 found valid input at line %i\niNumData = %i, Data =",__LINE__,*iNumData);
	if (*iNumData > 0 ) for (int idat=0;idat<*iNumData;idat++) dfprintf2(__LINE__,__FILE__,TRACE," %i,",Data[idat]);
	dfprintf2(__LINE__,__FILE__,TRACE,"\n");
#endif

	return(SUCCESS);
}



