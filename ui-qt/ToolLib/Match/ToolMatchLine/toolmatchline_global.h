#ifndef TOOLMATCHLINE_GLOBAL_H
#define TOOLMATCHLINE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLMATCHLINE_LIBRARY)
#  define TOOLMATCHLINESHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLMATCHLINESHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLMATCHLINE_GLOBAL_H
