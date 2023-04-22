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

    friend class HolonDesktopPrivate;
    friend class HolonThemePrivate;
    const QScopedPointer<HolonThemeStylePrivate> d_ptr;

public:
    enum PixelMetric
    {
        PM_TaskbarHeight = PM_CustomBase | 1,
        PM_TaskbarWidth = PM_CustomBase | 2,
        PM_SidebarSwitchButtonWidth = PM_CustomBase | 3
    };

    enum PrimitiveElement
    {
        PE_Taskbar = PE_CustomBase | 1,
        PE_TitleBar = PE_CustomBase | 2,
        PE_SidebarSwitchButton = PE_CustomBase | 3,
        PE_WindowAreaSwitchButton = PE_CustomBase | 4
    };

    explicit HolonThemeStyle(HolonTheme *theme);
    ~HolonThemeStyle();

    void adjustPanelWidgetPalette(QWidget *widget);

    void drawComplexControl(QStyle::ComplexControl control,
                            const QStyleOptionComplex *option,
                            QPainter *painter,
                            const QWidget *widget = nullptr) const override;

    void drawControl(QStyle::ControlElement element,
                     const QStyleOption *option,
                     QPainter *painter,
                     const QWidget *widget = nullptr) const override;

    void drawPrimitive(QStyle::PrimitiveElement element,
                       const QStyleOption *option,
                       QPainter *painter,
                       const QWidget *widget = nullptr) const override;

    void drawToolButtonSeparator(const QStyleOption *option,
                                 QPainter *painter) const;

    bool isPanelWidget(QWidget *widget) const;

    int pixelMetric(QStyle::PixelMetric metric,
                    const QStyleOption *option = nullptr,
                    const QWidget *widget = nullptr) const override;

    void polish(QWidget *widget) override;

    int styleHint(QStyle::StyleHint hint,
                  const QStyleOption *option = nullptr,
                  const QWidget *widget = nullptr,
                  QStyleHintReturn *returnData = nullptr) const override;

    HolonTheme *theme() const;
};

#endif // HOLONTHEMESTYLE_H
