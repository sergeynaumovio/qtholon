// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonthemestyle.h"
#include "holondesktop_p.h"
#include "holontheme.h"
#include "holonthemecolors.h"
#include "holonthemeicons.h"
#include "holonthemestyle_p.h"
#include "holontitlebar.h"
#include "holonwindowareaswitch.h"
#include <QAbstractButton>
#include <QApplication>
#include <QComboBox>
#include <QFormLayout>
#include <QLabel>
#include <QPainter>
#include <QStyleFactory>
#include <QStyleOption>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

static bool isMac()
{
#ifdef Q_OS_MAC
    return true;
#else
    return false;
#endif
}

HolonThemeStyle::HolonThemeStyle(HolonTheme *theme)
:   QProxyStyle(QStyleFactory::create(isMac() ? QApplication::style()->objectName() : u"fusion"_s)),
    d_ptr(new HolonThemeStylePrivate(this, theme))
{
    setParent(theme);
}

HolonThemeStyle::~HolonThemeStyle()
{ }

void HolonThemeStyle::adjustPanelWidgetPalette(QWidget *widget)
{
    QColor color;
    QPalette palette = widget->palette();

    color = theme()->colors()->panelTextColorLight();
    palette.setBrush(QPalette::All, QPalette::WindowText, color);
    palette.setBrush(QPalette::All, QPalette::ButtonText, color);

    color = theme()->colors()->iconsDisabledColor();
    palette.setBrush(QPalette::Disabled, QPalette::WindowText, color);
    palette.setBrush(QPalette::Disabled, QPalette::ButtonText, color);

    widget->setPalette(palette);
}

void HolonThemeStyle::drawComplexControl(QStyle::ComplexControl control,
                                         const QStyleOptionComplex *option,
                                         QPainter *painter,
                                         const QWidget *widget) const
{
    switch (control) {
    case CC_ComboBox:
        drawToolButtonSeparator(option, painter);
        painter->setClipRect(option->rect.adjusted(0, 0, -2, 0));
        drawPrimitive(PE_PanelButtonTool, option, painter, widget);
        drawPrimitive(PE_IndicatorArrowDown, option, painter, widget);
        drawPrimitive(PE_IndicatorArrowUp, option, painter, widget);
        break;
    default:
        QProxyStyle::drawComplexControl(control, option, painter, widget);
        break;
    }
}

void HolonThemeStyle::drawControl(QStyle::ControlElement element,
                                  const QStyleOption *option,
                                  QPainter *painter,
                                  const QWidget *widget) const
{
    switch (element) {
    case CE_ComboBoxLabel:
        if (const auto *cb = qstyleoption_cast<const QStyleOptionComboBox *>(option))
        {
            painter->save();
            QRect rect = subControlRect(CC_ComboBox, cb, SC_ComboBoxEditField, widget);
            painter->setPen(cb->state.testAnyFlag(State_Enabled) ? cb->palette.color(QPalette::WindowText)
                                                                 : theme()->colors()->iconsDisabledColor());
            int dy = (qRound(qApp->devicePixelRatio()) == 1 ? 2 : -2);
            painter->drawText(rect.adjusted(1, dy, 0, 0), Qt::AlignLeft | Qt::AlignVCenter, cb->currentText);
            painter->restore();
        }
        break;
    default:
        QProxyStyle::drawControl(element, option, painter, widget);
        break;
    }
}

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
                    painter->fillRect(option->rect, theme()->colors()->buttonPressedColor());
                else if (isHovered)
                    painter->fillRect(option->rect, theme()->colors()->buttonHoveredColor());
            }
            break;
        case PE_IndicatorArrowDown:
        case PE_IndicatorArrowUp: {
                int x = option->rect.width() - 18;
                int y = option->rect.height() / 2 - 7;
                int w = pixelMetric(QStyle::PM_ButtonIconSize);
                QIcon icon = (element == PE_IndicatorArrowDown ? theme()->icons()->indicatorArrowDownIcon()
                                                               : theme()->icons()->indicatorArrowUpIcon());
                icon.paint(painter, x, y, w, w);
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
                painter->save();

                QPen pen(theme()->colors()->panelTextColorLight());
                pen.setWidth(2);
                painter->setPen(pen);

                bool isHovered = option->state.testAnyFlag(QStyle::State_MouseOver);
                bool isChecked = option->state.testAnyFlag(QStyle::State_On);

                if (isHovered)
                {
                    if (isChecked)
                        painter->fillRect(option->rect, theme()->colors()->buttonPressedColor());
                    else
                        painter->fillRect(option->rect, theme()->colors()->buttonHoveredColor());
                }

                if (isChecked)
                {
                    if (!isHovered)
                        painter->fillRect(option->rect, theme()->colors()->buttonPressedHoveredColor());

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

                painter->restore();
            }
            break;
        default:
            break;
        }
    }
}

void HolonThemeStyle::drawToolButtonSeparator(const QStyleOption *option, QPainter *painter) const
{
    QPointF margin = QPointF(0, 3);
    QRectF rect = QRectF(option->rect).adjusted(1, 1, -1, -1);

    painter->save();
    painter->setPen(theme()->colors()->toolButtonSeparatorColor());
    painter->drawLine(rect.topRight() + margin, rect.bottomRight() - margin);
    painter->restore();
}

bool HolonThemeStyle::isPanelWidget(QWidget *widget) const
{
    if (!widget)
        return false;

    if (qobject_cast<HolonTitleBar *>(widget->parent()))
        return true;

    if (qobject_cast<HolonWindowAreaSwitch *>(widget->parent()))
        return true;

    return false;
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

void HolonThemeStyle::polish(QWidget *widget)
{
    QProxyStyle::polish(widget);

    if (!isPanelWidget(widget))
        return;

    if (qobject_cast<QToolButton *>(widget))
    {
        widget->setAttribute(Qt::WA_Hover);

        if (qobject_cast<HolonSwitchButton *>(widget))
        {
            int h = pixelMetric(static_cast<QStyle::PixelMetric>(PM_TaskbarHeight));
            int w = pixelMetric(static_cast<QStyle::PixelMetric>(PM_TaskbarWidth));

            if (d_ptr->desktop_d->taskbarArea() == HolonDesktopPrivate::TaskbarArea::Top ||
                d_ptr->desktop_d->taskbarArea() == HolonDesktopPrivate::TaskbarArea::Bottom)
            {
                w = pixelMetric(static_cast<QStyle::PixelMetric>(PM_SidebarSwitchButtonWidth));
                widget->setFixedHeight(h);
                widget->setFixedWidth(w);
            }
            else
            {
                widget->setFixedWidth(w);
                widget->setFixedHeight(w);
            }
        }

        return;
    }

    if (qobject_cast<QLabel *>(widget))
        adjustPanelWidgetPalette(widget);

    if (qobject_cast<QComboBox *>(widget))
    {
        adjustPanelWidgetPalette(widget);
        widget->setAttribute(Qt::WA_Hover);

        return;
    }
}

int HolonThemeStyle::styleHint(QStyle::StyleHint hint,
                               const QStyleOption *option,
                               const QWidget *widget,
                               QStyleHintReturn *returnData) const
{
    switch (hint) {
    case SH_FormLayoutFieldGrowthPolicy:
        return QFormLayout::AllNonFixedFieldsGrow;
    case SH_ComboBox_AllowWheelScrolling:
        return QGuiApplication::keyboardModifiers() == (isMac() ? Qt::MetaModifier : Qt::ControlModifier);
    default:
        break;
    }

    return QProxyStyle::styleHint(hint, option, widget, returnData);
}

HolonTheme *HolonThemeStyle::theme() const
{
    return d_ptr->theme;
}
