#ifndef CONTRAST_GLOBAL_H
#define CONTRAST_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CONTRAST_LIBRARY)
#  define CONTRASTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define CONTRASTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // CONTRAST_GLOBAL_H
