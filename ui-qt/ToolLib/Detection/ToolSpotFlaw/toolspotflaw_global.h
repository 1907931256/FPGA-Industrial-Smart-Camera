#ifndef TOOLSPOTFLAW_GLOBAL_H
#define TOOLSPOTFLAW_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TOOLSPOTFLAW_LIBRARY)
#  define TOOLSPOTFLAWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define TOOLSPOTFLAWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // TOOLSPOTFLAW_GLOBAL_H
