#ifndef LOGICIOJUMP_GLOBAL_H
#define LOGICIOJUMP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGICIOJUMP_LIBRARY)
#  define LOGICIOJUMPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGICIOJUMPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGICIOJUMP_GLOBAL_H
