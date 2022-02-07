/****************************************************************************
**
** Copyright (C) 2022 Sergey Naumov
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

#include <QStyleOption>
#include <QPainter>
#include <QHBoxLayout>
#include <QVBoxLayout>

class HolonTaskbarPrivate
{
public:
    HolonTaskbar *const q_ptr;
    HolonDesktop *const parent;
    HolonTaskbar::Area area;
    int preferedHeight;
    int preferedWidth;

    HolonTaskbarPrivate(HolonTaskbar *q, HolonDesktop *parent)
    :   q_ptr(q),
        parent(parent)
    { }

};

void HolonTaskbar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonTaskbar::HolonTaskbar(QLoaderSettings *settings, HolonDesktop *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonTaskbarPrivate(this, parent))
{
    if (!parent)
    {
        emitError("HolonDesktop not found");
        return;
    }

    d_ptr->preferedHeight = value("preferedHeight", 40).toInt();
    d_ptr->preferedWidth = value("preferedWidth", 40).toInt();

    QString area = value("area").toString();
    if (area == "left") d_ptr->area = Left;
    else if (area == "right") d_ptr->area = Right;
    else if (area == "top") d_ptr->area = Top;
    else d_ptr->area = Bottom;

    if (d_ptr->area == Left || d_ptr->area == Right)
    {
        setLayout(new QVBoxLayout(this));
        {
            layout()->setContentsMargins({});
            setFixedWidth(d_ptr->preferedWidth);
        }
    }
    else
    {
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            setFixedHeight(d_ptr->preferedHeight);
        }
    }

    parent->addTaskbar(this);
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
    return d_ptr->area;
}

HolonDesktop *HolonTaskbar::desktop() const
{
    return d_ptr->parent;
}

QBoxLayout *HolonTaskbar::layout() const
{
    return static_cast<QBoxLayout*>(QWidget::layout());
}

int HolonTaskbar::preferedHeight() const
{
    return d_ptr->preferedHeight;
}

int HolonTaskbar::preferedWidth() const
{
    return d_ptr->preferedWidth;
}
