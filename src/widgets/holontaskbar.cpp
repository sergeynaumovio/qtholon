/****************************************************************************
**
** Copyright (C) 2022-2023 Sergey Naumov
**
** Permission to use, copy, modify, and/or distribute this
** software for any purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
** THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
** LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**
****************************************************************************/

#include "holontaskbar.h"
#include "holondesktop_p.h"
#include "holonsidebarswitch.h"
#include "holontaskmenu.h"
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

    if (desktop_d.taskbarArea() == HolonDesktopPrivate::Area::Left ||
        desktop_d.taskbarArea() == HolonDesktopPrivate::Area::Right)
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
    setStyleSheet(desktop_d.taskbarStyleSheet());

    new HolonTaskMenu(desktop_d, this);
    d.sidebarSwitch = new HolonSidebarSwitch(desktop_d, this);
}

HolonTaskbar::~HolonTaskbar()
{ }

void HolonTaskbar::addStretch()
{
    layout()->addStretch();
}

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
