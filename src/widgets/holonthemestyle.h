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
    enum PixelMetric
    {
        PM_TaskbarHeight = PM_CustomBase | 1,
        PM_TaskbarWidth = PM_CustomBase | 2,
        PM_SidebarSwitchButtonWidth = PM_CustomBase | 3
    };

    ~HolonThemeStyle();

    void drawPrimitive(QStyle::PrimitiveElement element,
                       const QStyleOption *option,
                       QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    int pixelMetric(QStyle::PixelMetric metric,
                    const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;

    HolonTheme *theme() const;
};

#endif // HOLONTHEMESTYLE_H
