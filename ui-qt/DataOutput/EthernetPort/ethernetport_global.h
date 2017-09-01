#ifndef ETHERNETPORT_GLOBAL_H
#define ETHERNETPORT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(ETHERNETPORT_LIBRARY)
#  define ETHERNETPORTSHARED_EXPORT Q_DECL_EXPORT
#else
#  define ETHERNETPORTSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // ETHERNETPORT_GLOBAL_H
