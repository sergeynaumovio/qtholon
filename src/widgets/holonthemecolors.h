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

public:
    explicit HolonThemeColors(QLoaderSettings *settings, HolonTheme *parent);
    ~HolonThemeColors();

    virtual QColor baseColor() const;
    virtual QColor buttonHoveredColor() const;
    virtual QColor buttonPressedColor() const;
    virtual QColor buttonPressedHoveredColor() const;
    virtual QColor iconsBaseColor() const;
    virtual QColor iconsDisabledColor() const;
    virtual QColor iconsMenuColor() const;
    virtual QColor mainWindowSeparatorColor() const;
    virtual QColor panelTextColorDark() const;
    virtual QColor panelTextColorLight() const;
    HolonTheme *theme() const;
    virtual QColor toolButtonSeparatorColor() const;
};

#endif // HOLONTHEMECOLORS_H
