// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTHEMEICONS_H
#define HOLONTHEMEICONS_H

#include "holontheme.h"
#include "qtholonglobal.h"
#include <QIcon>
#include <QLoaderSettings>
#include <QObject>

class HolonTheme;
class HolonThemeColors;
class HolonThemeIconsPrivate;

using FileName = QString;

template<typename T>
class HolonMaskColor
{
public:
    T mask;
    QColor color;
};

class Q_HOLON_EXPORT HolonThemeIcons : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeIconsPrivate> d_ptr;

protected:
    HolonThemeIcons(QLoaderSettings *settings, HolonTheme *parent);

public:
    ~HolonThemeIcons();

    HolonThemeColors *colors() const;
    QIcon createIcon(const QList<HolonMaskColor<FileName>> &masks) const;
    virtual QIcon splitButtonCloseBottomIcon() const;
    virtual QIcon splitButtonHorizontalIcon() const;
    virtual QIcon splitButtonVerticalIcon() const;
    HolonTheme *theme() const;
};

#endif // HOLONTHEMEICONS_H
