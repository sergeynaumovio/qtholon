// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMESTYLESHEETS_H
#define HOLONTHEMESTYLESHEETS_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QObject>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIcons;
class HolonThemeStyleSheetsPrivate;

class Q_HOLON_EXPORT HolonThemeStyleSheets : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeStyleSheetsPrivate> d_ptr;

protected:
    HolonThemeStyleSheets(QLoaderSettings *settings, HolonTheme *parent);

public:
    ~HolonThemeStyleSheets();

    virtual QString buttonStyleSheet() const;
    HolonThemeColors *colors() const;
    HolonThemeIcons *icons() const;
    virtual QString mainWindowStyleSheet() const;
    virtual QString taskbarStyleSheet() const;
    HolonTheme *theme() const;
    virtual QString titleBarStyleSheet() const;
};

#endif // HOLONTHEMESTYLESHEETS_H
