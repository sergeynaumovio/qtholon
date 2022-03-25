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

#include "holonsidebarswitch.h"
#include "holontaskbar.h"
#include "holondesktop.h"
#include <QAbstractButton>
#include <QMouseEvent>
#include <QPainter>
#include <QBoxLayout>
#include <QLabel>

class HolonSidebarButton : public QAbstractButton
{
    bool hovered{};
    bool checked{};
    QChar c;
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
        p.setFont(QFont("Arial", 20));
        p.drawText(rect(), Qt::AlignCenter, c);
    }
public:
    HolonSidebarButton(QChar chr, HolonSidebarSwitch *parent)
    :   QAbstractButton(parent),
        c(chr)
    {
        if (parent->taskbar()->area() == HolonTaskbar::Top ||
            parent->taskbar()->area() == HolonTaskbar::Bottom)
        {
            int preferedWidth = parent->taskbar()->preferedHeight();
            setFixedWidth(preferedWidth + preferedWidth * 0.15);
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

HolonSidebarSwitch::HolonSidebarSwitch(QLoaderSettings *settings, HolonTaskbar *parent)
:   QWidget(parent),
    QLoaderSettings(settings)
{
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

    for (QChar chr : parent->desktop()->sidebarList())
    {
        layout()->addWidget(new HolonSidebarButton(chr, this));
    }

    parent->addWidget(this);
}

HolonDesktop *HolonSidebarSwitch::desktop() const
{
    return taskbar()->desktop();
}

HolonTaskbar *HolonSidebarSwitch::taskbar() const
{
    return static_cast<HolonTaskbar*>(parent());
}
