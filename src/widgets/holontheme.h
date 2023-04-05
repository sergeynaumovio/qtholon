// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEME_H
#define HOLONTHEME_H

#include "qtholonglobal.h"
#include <QColor>
#include <QLoaderSettings>
#include <QObject>

class HolonDesktop;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemePrivate;
class HolonThemeStyleSheets;

using FileName = QString;

template<typename T>
class HolonMaskColor
{
public:
    T mask;
    QColor color;
};

class Q_HOLON_EXPORT HolonTheme : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemePrivate> d_ptr;

protected:
    HolonTheme(QLoaderSettings *settings,
               HolonDesktop *desktop,
               HolonThemeColors *colors,
               HolonThemeIcons *icons,
               HolonThemeStyleSheets *styleSheets);

public:
    HolonTheme(QLoaderSettings *settings, HolonDesktop *desktop);
    ~HolonTheme();

    HolonThemeColors *colors() const;
    QIcon createIcon(const QList<HolonMaskColor<FileName>> &masks) const;
    HolonDesktop *desktop() const;
    HolonThemeIcons *icons() const;
    bool isCurrent() const;
    HolonThemeStyleSheets *styleSheets() const;
};

#endif // HOLONTHEME_H
