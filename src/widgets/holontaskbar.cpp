// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskbar.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonthemestyle.h"
#include "holonwindowareaswitch.h"
#include <QApplication>
#include <QBoxLayout>
#include <QPainter>
#include <QStyleOption>

class HolonTaskbarPrivate
{
public:
    HolonDesktopPrivate &desktop_d;
    HolonWindowAreaSwitch *windowAreaSwitch;

    HolonTaskbarPrivate(HolonDesktopPrivate &desk_d)
    :   desktop_d(desk_d)
    { }
};

void HolonTaskbar::paintEvent(QPaintEvent *)
{
    QStyle::PrimitiveElement pe = static_cast<QStyle::PrimitiveElement>(HolonThemeStyle::PE_Taskbar);
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(pe, &opt, &p, this);
}

HolonTaskbar::HolonTaskbar(HolonDesktopPrivate &desktop_d)
:   QWidget(desktop_d.q_ptr),
    d_ptr(desktop_d)
{
    if (desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Left ||
        desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Right)
    {
        setLayout(new QVBoxLayout(this));
    }
    else
        setLayout(new QHBoxLayout(this));

    layout()->setContentsMargins({});
    layout()->setSpacing(0);
    layout()->addWidget(d_ptr->windowAreaSwitch = new HolonWindowAreaSwitch(desktop_d, this));
    layout()->addStretch();
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

HolonWindowAreaSwitch *HolonTaskbar::sidebarSwitch() const
{
    return d_ptr->windowAreaSwitch;
}
