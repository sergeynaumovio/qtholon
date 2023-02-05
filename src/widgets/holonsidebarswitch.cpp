// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebarswitch.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include "holontaskbar.h"
#include <QAbstractButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>

class HolonSidebarSwitchPrivate
{
public:
    HolonDesktopPrivate &desktop_d;
    QMap<QString, QButtonGroup *> buttonGroupByName;
    QMap<QAbstractButton *, QButtonGroup *> buttonGroupByButton;
    QMap<QButtonGroup *, bool> toggledByButtonGroup;

    HolonSidebarSwitchPrivate(HolonDesktopPrivate &desk_d)
    :   desktop_d(desk_d)
    { }
};

class HolonSidebarButton : public QAbstractButton
{
    HolonSidebarSwitchPrivate &switch_d;
    HolonDesktopPrivate &desktop_d;
    bool hovered{};
    const QString title;
    const QColor color{Qt::white};

    bool isHovered() const { return hovered; }

protected:
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

public:
    HolonSidebarButton(HolonSidebarSwitchPrivate &swtch_d,
                       HolonSidebar *sidebar,
                       HolonSidebarSwitch *parent)
    :   QAbstractButton(parent),
        switch_d(swtch_d),
        desktop_d(switch_d.desktop_d),
        title(sidebar->title())
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
        setChecked(sidebar->isChecked());

        QShortcut *shortcut = new QShortcut(QKeySequence(sidebar->value("shortcut").toString()), this);
        connect(shortcut, &QShortcut::activated, this, [this]() { setChecked(!isChecked()); });

        HolonSidebarDock *dock = desktop_d.sidebarDock(sidebar);

        if (sidebar->group().size())
        {
            QButtonGroup *buttonGroup;
            if (switch_d.buttonGroupByName.contains(sidebar->group()))
            {
                buttonGroup = switch_d.buttonGroupByName.value(sidebar->group());
                switch_d.buttonGroupByButton.insert(this, buttonGroup);
                switch_d.toggledByButtonGroup.insert(buttonGroup, isChecked());
            }
            else
            {
                buttonGroup = new QButtonGroup(this);
                buttonGroup->setExclusive(false);
                switch_d.buttonGroupByName.insert(sidebar->group(), buttonGroup);
                switch_d.buttonGroupByButton.insert(this, buttonGroup);
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
                        {
                            switch_d.toggledByButtonGroup[buttonGroup] = true;
                            button->setChecked(false);
                        }
                    }
                }
                else
                {
                    if (switch_d.toggledByButtonGroup.value(buttonGroup))
                        switch_d.toggledByButtonGroup[buttonGroup] = false;

                    desktop_d.removeSidebar(sidebar);
                }
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

HolonSidebarSwitch::HolonSidebarSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *taskbar)
:   QWidget(taskbar),
    d(*new (&d_storage) HolonSidebarSwitchPrivate(desktop_d))
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

HolonSidebarSwitch::~HolonSidebarSwitch()
{
    d.~HolonSidebarSwitchPrivate();
}

void HolonSidebarSwitch::addSidebar(HolonSidebar *sidebar)
{
    layout()->addWidget(new HolonSidebarButton(d, sidebar, this));
}
