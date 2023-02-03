// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskbar.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebarswitch.h"
#include <QBoxLayout>
#include <QPainter>
#include <QStyleOption>

class HolonTaskbarPrivate
{
public:
    HolonDesktopPrivate &desktop_d;
    HolonSidebarSwitch *sidebarSwitch;

    HolonTaskbarPrivate(HolonDesktopPrivate &desk_d)
    :   desktop_d(desk_d)
    { }

};

void HolonTaskbar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonTaskbar::HolonTaskbar(HolonDesktopPrivate &desktop_d)
:   QWidget(desktop_d.q_ptr),
    d(*new (&d_storage) HolonTaskbarPrivate(desktop_d))
{
    static_assert (sizeof (d_storage) == sizeof (HolonTaskbarPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonTaskbarPrivate));

    if (desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Left ||
        desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Right)
    {
        setLayout(new QVBoxLayout(this));
        {
            layout()->setContentsMargins({});
            setFixedWidth(desktop_d.taskbarPreferedWidth());
        }
    }
    else
    {
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            setFixedHeight(desktop_d.taskbarPreferedHeight());
        }
    }
    layout()->setSpacing(0);
    //layout()->addWidget(new HolonTaskMenu(desktop_d, this));
    layout()->addWidget(d.sidebarSwitch = new HolonSidebarSwitch(desktop_d, this));
    layout()->addStretch();

    setStyleSheet(desktop_d.taskbarStyleSheet());
}

HolonTaskbar::~HolonTaskbar()
{ }

void HolonTaskbar::addWidget(QWidget *widget)
{
    layout()->addWidget(widget);
}

QBoxLayout *HolonTaskbar::layout() const
{
    return static_cast<QBoxLayout*>(QWidget::layout());
}

HolonSidebarSwitch *HolonTaskbar::sidebarSwitch() const
{
    return d.sidebarSwitch;
}
