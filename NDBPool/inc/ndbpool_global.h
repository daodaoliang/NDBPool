#ifndef NDBPOOL_GLOBAL_H
#define NDBPOOL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(NDBPOOL_LIBRARY)
#  define NDBPOOLSHARED_EXPORT Q_DECL_EXPORT
#else
#  define NDBPOOLSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // NDBPOOL_GLOBAL_H
