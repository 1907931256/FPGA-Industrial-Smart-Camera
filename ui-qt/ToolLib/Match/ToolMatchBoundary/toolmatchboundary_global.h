#ifndef TOOLMATCHBOUNDARY_GLOBAL_H
#define TOOLMATCHBOUNDARY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLMATCHBOUNDARY_LIBRARY)
#  define TOOLMATCHBOUNDARYSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLMATCHBOUNDARYSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLMATCHBOUNDARY_GLOBAL_H
