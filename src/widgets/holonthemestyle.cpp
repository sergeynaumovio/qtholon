// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestyle.h"
#include "holontaskbar.h"
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
    if (static_cast<unsigned int>(element) < static_cast<unsigned int>(PE_CustomBase))
    {
        switch (element) {
        case PE_IndicatorDockWidgetResizeHandle:
            painter->fillRect(option->rect, theme()->colors()->mainWindowSeparatorColor());
            break;
        default:
            break;
        }
    }
    else
    {
        HolonThemeStyle::PrimitiveElement custom = static_cast<HolonThemeStyle::PrimitiveElement>(element);
        switch (custom) {
        case PE_Taskbar:
            painter->fillRect(option->rect, theme()->colors()->baseColor());
            break;
        default:
            break;
        }
    }

    QProxyStyle::drawPrimitive(element, option, painter, widget);
}

int HolonThemeStyle::pixelMetric(QStyle::PixelMetric metric,
                                 const QStyleOption *option,
                                 const QWidget *widget) const
{
    if (static_cast<unsigned int>(metric) < static_cast<unsigned int>(QStyle::PM_CustomBase))
    {
        switch (metric) {
        case PM_DockWidgetSeparatorExtent:
            return 1;
        case PM_ToolBarIconSize:
        case PM_ListViewIconSize:
        case PM_ButtonIconSize:
        case PM_SmallIconSize:
            return 16;
        case PM_TitleBarHeight:
            return 24;
        default:
            break;
        }
    }
    else
    {
        HolonThemeStyle::PixelMetric custom = static_cast<HolonThemeStyle::PixelMetric>(metric);
        switch (custom) {
        case PM_TaskbarHeight:
            return 26;
        case PM_TaskbarWidth:
            return 50;
        case PM_SidebarSwitchButtonWidth:
            return 120;
        default:
            break;
        }
    }

    return QProxyStyle::pixelMetric(metric, option, widget);;
}

HolonTheme *HolonThemeStyle::theme() const
{
    return d_ptr->theme;
}
