#ifndef TOOLMONOAREA_GLOBAL_H
#define TOOLMONOAREA_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLMONOAREA_LIBRARY)
#  define TOOLMONOAREASHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLMONOAREASHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLMONOAREA_GLOBAL_H
