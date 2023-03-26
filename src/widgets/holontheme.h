// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEME_H
#define HOLONTHEME_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonDesktop;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemePrivate;

class Q_HOLON_EXPORT HolonTheme : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemePrivate> d_ptr;

protected:
    HolonTheme(QLoaderSettings *settings, HolonDesktop *desktop, HolonThemeColors *colors, HolonThemeIcons *icons);

public:
    HolonTheme(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonTheme();

    HolonThemeColors *colors() const;
    HolonThemeIcons *icons() const;
    bool isCurrent() const;
};

#endif // HOLONTHEME_H
