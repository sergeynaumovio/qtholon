// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMESTYLE_H
#define HOLONTHEMESTYLE_H

#include "qtholonglobal.h"
#include <QProxyStyle>

class HolonTheme;
class HolonThemeStylePrivate;

class Q_HOLON_EXPORT HolonThemeStyle : public QProxyStyle
{
    Q_OBJECT

    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeStylePrivate> d_ptr;

protected:
    HolonThemeStyle(HolonTheme *theme);

public:
    ~HolonThemeStyle();
};

#endif // HOLONTHEMESTYLE_H
