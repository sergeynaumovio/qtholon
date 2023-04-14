// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestyle.h"
#include "holontheme.h"
#include "holonthemecolors.h"
#include "holonthemestyle_p.h"
#include <QPainter>
#include <QStyleFactory>
#include <QStyleOption>

using namespace Qt::Literals::StringLiterals;

HolonThemeStyle::HolonThemeStyle(HolonTheme *theme) :
#ifdef Q_OS_MAC
    QProxyStyle(QStyleFactory::create(QApplication::style()->objectName())),
#else
    QProxyStyle(QStyleFactory::create(u"fusion"_s)),
#endif
    d_ptr(new HolonThemeStylePrivate(this, theme))
{ }

HolonThemeStyle::~HolonThemeStyle()
{ }

void HolonThemeStyle::drawPrimitive(QStyle::PrimitiveElement element,
                                    const QStyleOption *option,
                                    QPainter *painter,
                                    const QWidget *widget) const
{
    switch (element) {
    case PE_IndicatorDockWidgetResizeHandle:
        painter->fillRect(option->rect, theme()->colors()->mainWindowSeparatorColor());
        break;
    default:
        break;
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int HolonThemeStyle::pixelMetric(QStyle::PixelMetric metric,
                                 const QStyleOption *option,
                                 const QWidget *widget) const
{
    switch (metric) {
    case PM_DockWidgetSeparatorExtent:
        return 1;
    case PM_ToolBarIconSize:
    case PM_ListViewIconSize:
    case PM_ButtonIconSize:
    case PM_SmallIconSize:
        return 16;
    default:
        break;
    }

    return QProxyStyle::pixelMetric(metric, option, widget);;
}

HolonTheme *HolonThemeStyle::theme() const
{
    return d_ptr->theme;
}
