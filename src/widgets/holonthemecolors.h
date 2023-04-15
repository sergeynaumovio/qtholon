// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMECOLORS_H
#define HOLONTHEMECOLORS_H

#include "qtholonglobal.h"
#include <QColor>
#include <QLoaderSettings>
#include <QObject>

class HolonTheme;
class HolonThemeColorsPrivate;

class Q_HOLON_EXPORT HolonThemeColors : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeColorsPrivate> d_ptr;

protected:
    HolonThemeColors(QLoaderSettings *settings, HolonTheme *parent);

public:
    ~HolonThemeColors();

    virtual QColor baseColor() const;
    virtual QColor iconsBaseColor() const;
    virtual QColor iconsDisabledColor() const;
    virtual QColor mainWindowSeparatorColor() const;
    virtual QColor panelTextColorDark() const;
    virtual QColor panelTextColorLight() const;
    HolonTheme *theme() const;
};

#endif // HOLONTHEMECOLORS_H
