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
class HolonThemeStyleSheets;

class Q_HOLON_EXPORT HolonThemeIcons : public QObject, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeIconsPrivate> d_ptr;

public:
    explicit HolonThemeIcons(QLoaderSettings *settings, HolonTheme *parent);
    ~HolonThemeIcons();

    virtual QIcon closeBackgroundIcon() const;
    virtual QIcon closeForegroundIcon() const;
    virtual QIcon dirClosedIcon() const;
    virtual QIcon dirOpenIcon() const;
    virtual QIcon indicatorArrowDownIcon() const;
    virtual QIcon indicatorArrowUpIcon() const;
    virtual QIcon maximizeIcon() const;
    virtual QIcon minimizeIcon() const;
    virtual QIcon splitButtonCloseBottomIcon() const;
    virtual QIcon splitButtonCloseLeftIcon() const;
    virtual QIcon splitButtonCloseRightIcon() const;
    virtual QIcon splitButtonCloseTopIcon() const;
    virtual QIcon splitButtonHorizontalIcon() const;
    virtual QIcon splitButtonVerticalIcon() const;
    virtual QIcon taskIcon() const;
    HolonTheme *theme() const;
};

#endif // HOLONTHEMEICONS_H
