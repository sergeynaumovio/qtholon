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

#include "holonsidebarswitch.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holontaskbar.h"
#include <QAbstractButton>
#include <QBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>

class HolonSidebarSwitchPrivate
{
public:
    HolonTaskbar *const parent;
};

class HolonSidebarButton : public QAbstractButton
{
    HolonSidebarSwitch *const sidebarSwitch;
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
        QFont font("Arial", sidebarSwitch->taskbar()->preferedHeight() / 2.5);
        font.setBold(true);
        p.setFont(font);
        p.drawText(rect(), Qt::AlignCenter, title);
    }
public:
    HolonSidebarButton(const QString &string, HolonSidebarSwitch *parent)
    :   QAbstractButton(parent),
        sidebarSwitch(parent),
        title(string)
    {
        if (parent->taskbar()->area() == HolonTaskbar::Top ||
            parent->taskbar()->area() == HolonTaskbar::Bottom)
        {
            setFixedHeight(parent->taskbar()->preferedHeight());
        }
        else
        {
            setFixedWidth(parent->taskbar()->preferedWidth());
            setFixedHeight(parent->taskbar()->preferedWidth());
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

HolonSidebarSwitch::HolonSidebarSwitch(HolonTaskbar *parent)
:   QWidget(parent),
    d(*new (&d_storage) HolonSidebarSwitchPrivate{parent})
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarSwitchPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarSwitchPrivate));

    if (parent->area() == HolonTaskbar::Top ||
        parent->area() == HolonTaskbar::Bottom)
    {
        QBoxLayout *layout = new QHBoxLayout(this);
        {
            setLayout(layout);
            layout->setContentsMargins(5, 0, 5, 0);
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

    parent->addWidget(this);
}

void HolonSidebarSwitch::addSidebar(HolonSidebar *sidebar)
{
    layout()->addWidget(new HolonSidebarButton(sidebar->title(), this));
}

HolonDesktop *HolonSidebarSwitch::desktop() const
{
    return taskbar()->desktop();
}

HolonTaskbar *HolonSidebarSwitch::taskbar() const
{
    return d.parent;
}
