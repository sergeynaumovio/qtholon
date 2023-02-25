// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowareaswitch.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include "holontaskbar.h"
#include "holonwindowarea.h"
#include <QAbstractButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>

class HolonWindowAreaSwitchPrivate
{
public:
    HolonDesktopPrivate &desktop_d;

    QMap<QString, QButtonGroup *> sidebarButtonGroupByName;
    QButtonGroup *windowAreaButtonGroup{};

    HolonWindowAreaSwitchPrivate(HolonDesktopPrivate &desk_d)
    :   desktop_d(desk_d)
    { }
};

class HolonSwitchButton : public QAbstractButton
{
protected:
    HolonWindowAreaSwitchPrivate &switch_d;
    HolonDesktopPrivate &desktop_d;
    bool hovered{};
    const QString title;
    const QColor color{Qt::white};

    bool isHovered() const { return hovered; }

    bool event(QEvent *e) override
    {
        switch(e->type()) {
        case QEvent::HoverEnter:
            hovered = true;
            update();
            return true;
        case QEvent::HoverLeave:
            hovered = false;
            update();
            return true;
        default:
            return QWidget::event(e);
        }
    }

    void paintEvent(QPaintEvent *) override
    {
        QPainter p(this);
        QPen pen(color);
        pen.setWidth(2);
        p.setPen(pen);

        if (isHovered())
        {
            if (isChecked())
                p.fillRect(rect(), QColor(44, 46, 48));
            else
                p.fillRect(rect(), QColor(74, 76, 78));
        }

        if (isChecked())
        {
            if (!isHovered())
                p.fillRect(rect(), QColor(24, 26, 28));

            QLineF line(rect().bottomLeft(), rect().bottomRight());
            p.drawLine(line);
        }

        QFont font("Arial", desktop_d.taskbarPreferedHeight() / 2.6);
        p.setFont(font);
        QRect rectangle = rect();
        rectangle.adjust(10, 0, 0, 0);
        p.drawText(rectangle, Qt::AlignLeft | Qt::AlignVCenter, title);
    }

    HolonSwitchButton(HolonWindowAreaSwitchPrivate &swtch_d,
                      HolonWindowArea *windowArea,
                      HolonWindowAreaSwitch *parent)
    :   QAbstractButton(parent),
        switch_d(swtch_d),
        desktop_d(switch_d.desktop_d),
        title(windowArea->title())
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
        setChecked(windowArea->isChecked());

        QShortcut *shortcut = new QShortcut(QKeySequence(windowArea->shortcut()), this);
        connect(shortcut, &QShortcut::activated, this, [this]() { setChecked(!isChecked()); });
    }
};

class HolonSidebarButton : public HolonSwitchButton
{
public:
    HolonSidebarButton(HolonWindowAreaSwitchPrivate &swtch_d,
                       HolonSidebar *sidebar,
                       HolonWindowAreaSwitch *parent)
    :   HolonSwitchButton(swtch_d, sidebar, parent)
    {
        HolonSidebarDock *dock = desktop_d.sidebarDock(sidebar);

        if (sidebar->group().size())
        {
            QButtonGroup *buttonGroup;
            if (switch_d.sidebarButtonGroupByName.contains(sidebar->group()))
                buttonGroup = switch_d.sidebarButtonGroupByName.value(sidebar->group());
            else
            {
                buttonGroup = new QButtonGroup(this);
                buttonGroup->setExclusive(false);
                switch_d.sidebarButtonGroupByName.insert(sidebar->group(), buttonGroup);
            }
            buttonGroup->addButton(this);

            connect(this, &QAbstractButton::toggled, dock, [=, this](bool checked)
            {
                if (checked)
                {
                    desktop_d.restoreSidebar(sidebar);

                    for (QAbstractButton *button : buttonGroup->buttons())
                    {
                        if (button != this && button->isChecked())
                            button->setChecked(false);
                    }
                }
                else
                    desktop_d.removeSidebar(sidebar);
            });
        }
        else
        {
            connect(this, &QAbstractButton::toggled, dock, [=, this](bool checked)
            {
                if (checked)
                    desktop_d.restoreSidebar(sidebar);
                else
                    desktop_d.removeSidebar(sidebar);
            });
        }
    }
};

class HolonWindowAreaButton : public HolonSwitchButton
{
public:
    HolonWindowAreaButton(HolonWindowAreaSwitchPrivate &swtch_d,
                          HolonWindowArea *windowArea,
                          HolonWindowAreaSwitch *parent)
    :   HolonSwitchButton(swtch_d, windowArea, parent)
    {
        if (!switch_d.windowAreaButtonGroup)
        {
            switch_d.windowAreaButtonGroup = new QButtonGroup(this);
            switch_d.windowAreaButtonGroup->setExclusive(false);
        }
        switch_d.windowAreaButtonGroup->addButton(this);

        connect(this, &QAbstractButton::toggled, windowArea, [=, this](bool checked)
        {
            if (checked)
            {
                desktop_d.q_ptr->setCurrentWindowArea(windowArea);

                for (QAbstractButton *button : switch_d.windowAreaButtonGroup->buttons())
                {
                    if (button != this && button->isChecked())
                        button->setChecked(false);
                }
            }
            else if (!switch_d.windowAreaButtonGroup->checkedButton())
                desktop_d.q_ptr->setCurrentWindowArea(nullptr);
        });
    }
};

HolonWindowAreaSwitch::HolonWindowAreaSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *taskbar)
:   QWidget(taskbar),
    d(*new (&d_storage) HolonWindowAreaSwitchPrivate(desktop_d))
{
    static_assert (sizeof (d_storage) == sizeof (HolonWindowAreaSwitchPrivate));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonWindowAreaSwitchPrivate));

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

HolonWindowAreaSwitch::~HolonWindowAreaSwitch()
{
    d.~HolonWindowAreaSwitchPrivate();
}

void HolonWindowAreaSwitch::addSidebar(HolonSidebar *sidebar)
{
    layout()->addWidget(new HolonSidebarButton(d, sidebar, this));
}

void HolonWindowAreaSwitch::addWindowArea(HolonWindowArea *windowArea)
{
    layout()->addWidget(new HolonWindowAreaButton(d, windowArea, this));
}
