#ifndef _DEBUG_DMSG_H_
#define _DEBUG_DMSG_H_
//=============================================================================

#ifdef QT_CORE_LIB
#include <QtCore>
#define dmsg(...)       qDebug(__VA_ARGS__)

#else

#include <stdio.h>
#include <stdlib.h>
#define dmsg(...)       printf(__VA_ARGS__)
#endif

//=============================================================================
#endif /* _DEBUG_H_ */
