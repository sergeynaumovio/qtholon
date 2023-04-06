// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMESIZEHINTS_H
#define HOLONTHEMESIZEHINTS_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonTheme;
class HolonThemeSizeHintsPrivate;

class Q_HOLON_EXPORT HolonThemeSizeHints : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeSizeHintsPrivate> d_ptr;

protected:
    HolonThemeSizeHints(QLoaderSettings *settings, HolonTheme *parent);

public:
    ~HolonThemeSizeHints();

    virtual QSize iconsSizeHint() const;
    HolonTheme *theme() const;
    virtual QSize titleBarSizeHint() const;
};

#endif // HOLONTHEMESIZEHINTS_H
