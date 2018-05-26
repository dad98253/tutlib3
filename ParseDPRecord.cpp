/*
 * ParseDPRecord.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include <string.h>
#include <assert.h>
#include "tutlib.h"

int ParseDPRecord ( char * lpsEditedResource , int iLenResource , DPINFO *DPInfo , char ** szDefaultInput , char ** szDPIDString , char ** szDPText ) {

	int iCharOffset = 0;
	int iNextCR;
	Byte ucFirstChar;
	//	struct DPINFO {
	//		bool bVariableNumberOfFields = false;		// true => V, false => F
	//		int iNumFieldsAllowed = 0;
	//		int iTypeOfData = 0;		// 1 => char *, 2 => int, 3 => string, 4 => any
	//		int iStartRange = 0;
	//		int iEndRange = 0;
	//		int iFieldNumber = 0;	// number of fields found on the *WRET card
	//		bool bTo = false;		// true => TO, false => OR or none
	//	 };

// save some memory for the DP text
	if ( (*szDPText = (char *)malloc(iLenResource)) == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"malloc for szDPText in ParseDPRecord failed at line %i\n",__LINE__);
#endif
		return(-1);
	}
	int iOutTextOffset = 0;

// parse the *WRET, *WDEF, and *WDPI info
	while (iCharOffset < iLenResource) {
		ucFirstChar = (Byte)*(lpsEditedResource+iCharOffset);
		iNextCR = FindNextReturn(lpsEditedResource+iCharOffset+1);
		switch (ucFirstChar) {

			case 0202:		//  *WRET
			{
				char * str = (char *)malloc(iNextCR+1);
				strncpy(str,(const char *)(lpsEditedResource+iCharOffset+1),iNextCR);
				str[iNextCR] = '\000';
				char * pch;
				pch = strtok (str," ,");
				while (pch != NULL)
				{
					DPInfo->iFieldNumber++;
					switch (DPInfo->iFieldNumber) {

						case 1:
						{
							DPInfo->bVariableNumberOfFields = false;
							if ( strcmp(pch,"V") == 0 ) DPInfo->bVariableNumberOfFields = true;
							break;
						}

						case 2:
						{
							sscanf(pch,"%i",&(DPInfo->iNumFieldsAllowed));
							break;
						}

						case 3:
						{
							DPInfo->iTypeOfData = 1;
							if ( strcmp(pch,"INT") == 0 ) DPInfo->iTypeOfData = 2;
							if ( strcmp(pch,"STR") == 0 ) DPInfo->iTypeOfData = 3;
							if ( strcmp(pch,"ANY") == 0 ) DPInfo->iTypeOfData = 4;
							break;
						}

						case 4:
						{
							sscanf(pch,"%i",&(DPInfo->iStartRange));
							DPInfo->iEndRange = DPInfo->iStartRange;
							break;
						}

						case 5:
						{
							DPInfo->bTo = false;
							if ( strcmp(pch,"TO") == 0 ) DPInfo->bTo = true;
							break;
						}

						case 6:
						{
							sscanf(pch,"%i",&(DPInfo->iEndRange));
							break;
						}

						default:
						{
							break;
						}

					}		// end switch on iFieldNumber
					pch = strtok (NULL, " ,");
				}		// end while pch != NULL
				free(str);
				break;
			}

			case 0203:		//  *WDEF
			{
				*szDefaultInput = (char *)malloc(iNextCR+1);
				strncpy(*szDefaultInput,(const char *)(lpsEditedResource+iCharOffset+1),iNextCR);
				(*szDefaultInput)[iNextCR] = '\000';
				int iFirtstNonBlankChar = (int)strspn ( (const char *)*szDefaultInput, " " );
				assert((iFirtstNonBlankChar-1)<(int)strlen(*szDefaultInput));
				if (iFirtstNonBlankChar == (int)strlen(*szDefaultInput) || (*szDefaultInput)[iFirtstNonBlankChar] == ';' ) {
					(*szDefaultInput)[0] = '\000';
					break;
				}
				if (iFirtstNonBlankChar != 0) {
					assert((iNextCR-iFirtstNonBlankChar) >= 0);
					if((iNextCR-iFirtstNonBlankChar) < 0) iFirtstNonBlankChar = iNextCR;
					strncpy(*szDefaultInput,(const char *)(lpsEditedResource+iCharOffset+1+iFirtstNonBlankChar),iNextCR-iFirtstNonBlankChar);
					(*szDefaultInput)[iNextCR-iFirtstNonBlankChar] = '\000';
				}
				break;
			}

			case 0204:		//  *WDPI
			{
				*szDPIDString = (char *)malloc(iNextCR+1);
				strncpy(*szDPIDString,(const char *)(lpsEditedResource+iCharOffset+1),iNextCR);
				(*szDPIDString)[iNextCR] = '\000';
				int iFirtstNonBlankChar = (int)strspn ( (const char *)*szDPIDString, " " );
				assert((iFirtstNonBlankChar-1)<(int)strlen(*szDPIDString));
				if (iFirtstNonBlankChar == (int)strlen(*szDPIDString)) {
					(*szDPIDString)[0] = '\000';
					break;
				}
				if (iFirtstNonBlankChar != 0) {
					strncpy(*szDPIDString,(const char *)(lpsEditedResource+iCharOffset+1+iFirtstNonBlankChar),iNextCR-iFirtstNonBlankChar);
					assert((iNextCR-iFirtstNonBlankChar) >= 0);
					if((iNextCR-iFirtstNonBlankChar) < 0) iFirtstNonBlankChar = iNextCR;
					(*szDPIDString)[iNextCR-iFirtstNonBlankChar] = '\000';
				}
				break;
			}

			default:
			{
				strncpy((*szDPText)+iOutTextOffset,(const char *)(lpsEditedResource+iCharOffset),iNextCR+2);
				iOutTextOffset+= (iNextCR+2);
				break;
			}
		}	// end of switch
		iCharOffset += (iNextCR+2);
	}	// end of while
	*((*szDPText)+iOutTextOffset) = '\000';

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"\33[1;35mparse of display point data\33[0m\n");
	char * szftxed =(char *)"fixed";
	if (DPInfo->bVariableNumberOfFields) szftxed =(char *)"variable";
	char * szdatatype =(char *)"character";
	if (DPInfo->iTypeOfData == 2) szdatatype =(char *)"integer";
	if (DPInfo->iTypeOfData == 3) szdatatype =(char *)"string";
	if (DPInfo->iTypeOfData == 4) szdatatype =(char *)"any";
	char * szTO = (char *)"Or";
	if (DPInfo->bTo) szTO = (char *)"To";
	if (*szDPIDString != NULL)dfprintf(__LINE__,__FILE__,TRACE,"Display Point ID = \"%s\"\n",*szDPIDString);
	dfprintf(__LINE__,__FILE__,TRACE,"%s number of %s fields, %i allowed\n",szftxed,szdatatype,DPInfo->iNumFieldsAllowed);
	if (DPInfo->iFieldNumber>3)dfprintf(__LINE__,__FILE__,TRACE,"valid inputs: %i %s %i\n",DPInfo->iStartRange,szTO,DPInfo->iEndRange);
	if (*szDefaultInput != NULL)dfprintf(__LINE__,__FILE__,TRACE,"default input = \"%s\"\n",*szDefaultInput);
#endif

	return (0);
}



