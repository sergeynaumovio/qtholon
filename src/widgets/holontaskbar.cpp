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
#include "holondesktop.h"
#include "holonsidebarswitch.h"
#include "holontaskmenu.h"
#include <QBoxLayout>
#include <QPainter>
#include <QStyleOption>

class HolonTaskbarPrivate
{
public:
    HolonTaskbar::Area area;
    const int preferedWidth;
    const int preferedHeight;
    HolonDesktop *const desktop;
    HolonSidebarSwitch *sidebarSwitch;

    HolonTaskbarPrivate(HolonTaskbar::Area a,
                        int prefWidth,
                        int prefHeight,
                        HolonDesktop *desk)
    :   area(a),
        preferedWidth(prefWidth),
        preferedHeight(prefHeight),
        desktop(desk)
    { }

};

void HolonTaskbar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonTaskbar::HolonTaskbar(HolonTaskbar::Area area,
                           int preferedWidth,
                           int preferedHeight,
                           const QString &styleSheet,
                           HolonDesktop *desktop)
:   QWidget(desktop),
    d(*new (&d_storage) HolonTaskbarPrivate(area, preferedWidth, preferedHeight, desktop))
{
    static_assert (sizeof (d_storage) == sizeof (HolonTaskbarPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonTaskbarPrivate));

    if (area == HolonTaskbar::Left ||
        area == HolonTaskbar::Right)
    {
        setLayout(new QVBoxLayout(this));
        {
            layout()->setContentsMargins({});
            setFixedWidth(preferedWidth);
        }
    }
    else
    {
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            setFixedHeight(preferedHeight);
        }
    }
    setStyleSheet(styleSheet);

    new HolonTaskMenu(this);
    d.sidebarSwitch = new HolonSidebarSwitch(this);
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

HolonTaskbar::Area HolonTaskbar::area() const
{
    return d.area;
}

int HolonTaskbar::preferedHeight() const
{
    return d.preferedHeight;
}

int HolonTaskbar::preferedWidth() const
{
    return d.preferedWidth;
}

HolonDesktop *HolonTaskbar::desktop() const
{
    return d.desktop;
}

QBoxLayout *HolonTaskbar::layout() const
{
    return static_cast<QBoxLayout*>(QWidget::layout());
}

HolonSidebarSwitch *HolonTaskbar::sidebarSwitch() const
{
    return d.sidebarSwitch;
}
