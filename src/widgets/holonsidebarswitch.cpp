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
        QFont font("Arial", desktop_d.taskbarPreferedHeight() / 2.5);
        font.setBold(true);
        p.setFont(font);
        p.drawText(rect(), Qt::AlignCenter, title);
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

    taskbar->addWidget(this);
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

    QShortcut *shortcut = new QShortcut(QKeySequence(sidebar->value("shortcut").toString()), this);
    connect(shortcut, &QShortcut::activated, this, [sidebarButton]()
    {
        sidebarButton->setChecked(!sidebarButton->isChecked());
    });
}

