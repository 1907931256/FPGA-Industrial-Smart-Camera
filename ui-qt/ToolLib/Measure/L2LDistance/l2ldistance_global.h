#ifndef L2LDISTANCE_GLOBAL_H
#define L2LDISTANCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(L2LDISTANCE_LIBRARY)
#  define L2LDISTANCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define L2LDISTANCESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // L2LDISTANCE_GLOBAL_H