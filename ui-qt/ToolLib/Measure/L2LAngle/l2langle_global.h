#ifndef L2LANGLE_GLOBAL_H
#define L2LANGLE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(L2LANGLE_LIBRARY)
#  define L2LANGLESHARED_EXPORT Q_DECL_EXPORT
#else
#  define L2LANGLESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // L2LANGLE_GLOBAL_H
