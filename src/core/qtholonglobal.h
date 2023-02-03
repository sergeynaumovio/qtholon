// Copyright (C) 2022 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef QTHOLONGLOBAL_H
#define QTHOLONGLOBAL_H

#include <QtCore/qcompilerdetection.h>

#ifndef QT_STATIC
#  if defined(QT_BUILD_HOLON_LIB)
#    define Q_HOLON_EXPORT Q_DECL_EXPORT
#  else
#    define Q_HOLON_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define Q_HOLON_EXPORT
#endif

#endif // QTHOLONGLOBAL_H
