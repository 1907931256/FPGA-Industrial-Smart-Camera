#ifndef LOGICCMPJUMP_GLOBAL_H
#define LOGICCMPJUMP_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LOGICCMPJUMP_LIBRARY)
#  define LOGICCMPJUMPSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LOGICCMPJUMPSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LOGICCMPJUMP_GLOBAL_H
