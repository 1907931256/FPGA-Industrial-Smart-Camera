#ifndef P2PDISTANCE_GLOBAL_H
#define P2PDISTANCE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(P2PDISTANCE_LIBRARY)
#  define P2PDISTANCESHARED_EXPORT Q_DECL_EXPORT
#else
#  define P2PDISTANCESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // P2PDISTANCE_GLOBAL_H
