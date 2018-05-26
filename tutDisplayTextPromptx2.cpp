/*
 * tutDisplayTextPromptx2.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"
#include "tuterrors.h"

int tutDisplayTextPrompt ( int iDisplayPointID , int NumReplacements , ...  ) {
	va_list ap;
    va_start(ap, NumReplacements);

#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"tutDisplayTextPromptx2 called for \"%i\"\n",iDisplayPointID);
#endif

    int iRet = tutDisplayWorker ( iDisplayPointID , DISPLAYTEXTPROMPT , NumReplacements , ap );
	va_end(ap);

	if ( sztutNextLine == NULL ) {
#ifdef DEBUG
		dfprintf(__LINE__,__FILE__,TRACE,"sztutNextLine not defined in tutDisplayTextPromptx2 at line %i, iRet = %i\n",__LINE__,iRet);
#endif
		return(RET_ABORTPROGRAM);
	}
	return(iRet);
}




