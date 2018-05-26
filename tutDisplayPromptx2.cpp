/*
 * tutDisplayPrompt.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#include "tutlib.h"

int tutDisplayPrompt (int iDisplayPointID, int NumReplacements, ...) {
	va_list ap;
	va_start(ap, NumReplacements);
#ifdef DEBUG
	dfprintf(__LINE__,__FILE__,TRACE,"tutDisplayPromptx2 called for \"%i\"\n",iDisplayPointID);
#endif
    int iRet = tutDisplayWorker (iDisplayPointID , DISPLAYPROMPT , NumReplacements , ap );
	va_end(ap);
	return(iRet);

}




