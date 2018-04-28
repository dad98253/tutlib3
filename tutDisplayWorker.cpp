/*
 * tutDisplayWorker.cpp
 *
 *  Created on: Jun 10, 2016
 *      Author: dad
 */

#ifdef WINDOZE
#include <stdarg.h>
#endif
#include "tutlib.h"

int tutDisplayWorker ( int iDisplayPointID, int iCallerID, int NumReplacements , va_list ap  ) {
//	va_list ap;
//	va_start(ap, NumReplacements);

#ifdef DEBUG
	if (debugflag) dfprintf(fp9,"tutDisplayWorker called with iCallerID = %i\n",iCallerID);
#endif

    int iRet = tutDisplayWorkerL2 (iDisplayPointID , iCallerID , NumReplacements , ap );

	va_end(ap);
	return(iRet);

}


