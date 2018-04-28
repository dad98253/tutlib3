/*
 * TutStoreRecData.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */


extern int MsgBox(const char * message, ...);

// store a value routine
bool TutStoreRecData (int iNewData, int * Data,int * iNumData, int iSizeData)
{

//check range (if required)
//	check for duplicate
	if ( *iNumData > 0 ) {
		for(int i=0;i<(*iNumData);i++) if ( iNewData == Data[i] ) return (true);
		if ((*iNumData)+1 > iSizeData ) {
			MsgBox("Too many items specified.\nThe maximum number of items that can be stored using this type of input is %i.",iSizeData);
			return(false);
		}
	}

	Data[*iNumData] = iNewData;
	(*iNumData)++;

	return(true);
}


