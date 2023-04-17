// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestyle.h"
#include "holontheme.h"
#include "holonthemecolors.h"
#include "holonthemestyle_p.h"
#include <QAbstractButton>
#include <QApplication>
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
        case PE_PanelButtonTool: {
                bool isPressed = option->state.testAnyFlags({State_Sunken, State_On});
                bool isHovered = option->state.testFlags({State_Enabled, State_MouseOver});

                if (isPressed)
                    painter->fillRect(option->rect, QColor(44, 46, 48));
                else if (isHovered)
                    painter->fillRect(option->rect, QColor(74, 76, 78));
            }
            break;
        default:
            QProxyStyle::drawPrimitive(element, option, painter, widget);
            break;
        }
    }
    else
    {
        HolonThemeStyle::PrimitiveElement custom = static_cast<HolonThemeStyle::PrimitiveElement>(element);
        switch (custom) {
        case PE_Taskbar:
        case PE_TitleBar:
            painter->fillRect(option->rect, theme()->colors()->baseColor());
            break;
        case PE_SidebarSwitchButton:
        case PE_WindowAreaSwitchButton: {
                QPen pen(Qt::white);
                pen.setWidth(2);
                painter->setPen(pen);

                bool isHovered = option->state.testAnyFlag(QStyle::State_MouseOver);
                bool isChecked = option->state.testAnyFlag(QStyle::State_On);

                if (isHovered)
                {
                    if (isChecked)
                        painter->fillRect(option->rect, QColor(44, 46, 48));
                    else
                        painter->fillRect(option->rect, QColor(74, 76, 78));
                }

                if (isChecked)
                {
                    if (!isHovered)
                        painter->fillRect(option->rect, QColor(24, 26, 28));

                    QLineF line(option->rect.bottomLeft(), option->rect.bottomRight());
                    painter->drawLine(line);
                }

                if (const QAbstractButton *button = qobject_cast<const QAbstractButton *>(widget))
                {
                    painter->setFont(QApplication::font());
                    QRect rectangle = option->rect;
                    rectangle.adjust(10, 0, 0, 0);
                    painter->drawText(rectangle, Qt::AlignLeft | Qt::AlignVCenter, button->text());
                }
            }
            break;
        default:
            break;
        }
    }
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
        case PM_MenuButtonIndicator:
            return 0;
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
