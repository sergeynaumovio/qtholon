// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebarswitch.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include "holontaskbar.h"
#include <QAbstractButton>
#include <QBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>

class HolonSidebarSwitchPrivate
{
public:
    HolonDesktopPrivate &desktop_d;
};

class HolonSidebarButton : public QAbstractButton
{
    HolonDesktopPrivate &desktop_d;
    bool hovered{};
    bool checked{};
    const QString title;
    const QColor color{Qt::white};

protected:
    bool event(QEvent *e) override
    {
        switch(e->type()) {
        case QEvent::HoverEnter:
            hovered = true;
            update();
            return true;
            break;
        case QEvent::HoverLeave:
            hovered = false;
            update();
            return true;
            break;
        default:
            break;
        }

        return QWidget::event(e);
    }

    void paintEvent(QPaintEvent *) override
    {

        QPainter p(this);
        QPen pen(color);
        pen.setWidth(2);
        p.setPen(pen);

        if (hovered && !checked)
            p.fillRect(rect(), QColor(74, 76, 78));

        if (hovered && checked)
            p.fillRect(rect(), QColor(44, 46, 48));

        if (checked)
        {
            if (!hovered)
                p.fillRect(rect(), QColor(24, 26, 28));

            QLineF line(rect().bottomLeft(), rect().bottomRight());
            p.drawLine(line);
        }
        QFont font("Arial", desktop_d.taskbarPreferedHeight() / 2.6 );
        p.setFont(font);
        QRect rectangle = rect();
        rectangle.adjust(10, 0, 0, 0);
        p.drawText(rectangle, Qt::AlignLeft | Qt::AlignVCenter, title);
    }
public:
    HolonSidebarButton(HolonDesktopPrivate &desk_d,
                       const QString &buttonTitle,
                       HolonSidebarSwitch *parent)
    :   QAbstractButton(parent),
        desktop_d(desk_d),
        title(buttonTitle)
    {
        if (desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Top ||
            desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Bottom)
        {
            setFixedHeight(desktop_d.taskbarPreferedHeight());
            setFixedWidth(desktop_d.sidebarSwitchButtonWidth());
        }
        else
        {
            setFixedWidth(desktop_d.taskbarPreferedWidth());
            setFixedHeight(desktop_d.taskbarPreferedWidth());
        }

        setAttribute(Qt::WA_Hover);
        setCheckable(true);
        connect(this, &QAbstractButton::pressed, this, [this]
        {
            checked = !checked;
            update();
        });
    }
};

HolonSidebarSwitch::HolonSidebarSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *taskbar)
:   QWidget(taskbar),
    d(*new (&d_storage) HolonSidebarSwitchPrivate{desktop_d})
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarSwitchPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarSwitchPrivate));

    if (desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Top ||
        desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Bottom)
    {
        QBoxLayout *layout = new QHBoxLayout(this);
        {
            setLayout(layout);
            layout->setContentsMargins({4, 0, 0, 0});
            layout->setSpacing(4);
        }
    }
    else
    {
        QBoxLayout *layout = new QVBoxLayout(this);
        {
            setLayout(layout);
            layout->setContentsMargins(0, 5, 0, 5);
        }
    }
}

void HolonSidebarSwitch::addSidebar(HolonSidebar *sidebar)
{
    HolonSidebarButton *sidebarButton = new HolonSidebarButton(d.desktop_d, sidebar->title(), this);
    layout()->addWidget(sidebarButton);
    HolonSidebarDock *dock = d.desktop_d.sidebarDock(sidebar);

    connect(sidebarButton, &QAbstractButton::toggled, d.desktop_d.sidebarDock(sidebar), [dock, this](bool checked)
    {
        if (checked)
            d.desktop_d.restoreDockWidget(dock);
        else
            d.desktop_d.removeDockWidget(dock);

        d.desktop_d.resizeDocks();
    });

    if (sidebar->isChecked())
        emit sidebarButton->pressed();

    QShortcut *shortcut = new QShortcut(QKeySequence(sidebar->value("shortcut").toString()), this);
    connect(shortcut, &QShortcut::activated, this, [sidebarButton]()
    {
        sidebarButton->setChecked(!sidebarButton->isChecked());
    });
}

