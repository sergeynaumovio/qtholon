// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowareaswitch.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardockwidget.h"
#include "holontaskbar.h"
#include "holonthemestyle.h"
#include "holonwindowarea.h"
#include <QApplication>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QMouseEvent>
#include <QShortcut>
#include <QStyleOption>
#include <QStylePainter>

using namespace Qt::Literals::StringLiterals;

class HolonWindowAreaSwitchPrivate
{
public:
    HolonDesktopPrivate &desktop_d;

    QMap<QString, QButtonGroup *> sidebarButtonGroupByName;
    QButtonGroup *windowAreaButtonGroup{};
    QMap<HolonWindowArea *, QToolButton *> buttonByWindowArea;

    HolonWindowAreaSwitchPrivate(HolonDesktopPrivate &desk_d)
    :   desktop_d(desk_d)
    { }

    void closeWindowArea(HolonWindowArea *windowArea)
    {
        if (QToolButton *button = buttonByWindowArea.value(windowArea))
            button->setChecked(false);
    }
};

HolonWindowAreaSwitch::HolonWindowAreaSwitch(HolonDesktopPrivate &desktop_d, HolonTaskbar *taskbar)
:   QWidget(taskbar),
    d_ptr(desktop_d)
{
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
{ }

void HolonWindowAreaSwitch::addSidebar(HolonSidebar *sidebar)
{
    layout()->addWidget(new HolonSidebarButton(*d_ptr, sidebar, this));
}

void HolonWindowAreaSwitch::addWindowArea(HolonWindowArea *windowArea)
{
    layout()->addWidget(new HolonWindowAreaButton(*d_ptr, windowArea, this));
}

void HolonWindowAreaSwitch::closeWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->closeWindowArea(windowArea);
}

void HolonSwitchButton::paintEvent(QPaintEvent *)
{
    QStyleOption option;
    option.initFrom(this);
    option.state.setFlag(isChecked() ? QStyle::State_On : QStyle::State_Off);
    QStylePainter(this).drawPrimitive(pe, option);
}

HolonSwitchButton::HolonSwitchButton(HolonWindowAreaSwitchPrivate &swtch_d,
                                     HolonWindowArea *windowArea,
                                     HolonWindowAreaSwitch *parent)
:   QToolButton(parent),
    pe(static_cast<QStyle::PrimitiveElement>(qobject_cast<HolonSidebar *>(windowArea) ?
                                             HolonThemeStyle::PE_SidebarSwitchButton :
                                             HolonThemeStyle::PE_WindowAreaSwitchButton)),
    switch_d(swtch_d),
    desktop_d(switch_d.desktop_d)
{
    setCheckable(true);
    setChecked(windowArea->isChecked());
    setMinimumSize(1, 1);
    setText(windowArea->title());

    QShortcut *shortcut = new QShortcut(QKeySequence(windowArea->shortcut()), this);
    connect(shortcut, &QShortcut::activated, this, [this]() { setChecked(!isChecked()); });
}

HolonSidebarButton::HolonSidebarButton(HolonWindowAreaSwitchPrivate &swtch_d,
                                       HolonSidebar *sidebar,
                                       HolonWindowAreaSwitch *parent)
:   HolonSwitchButton(swtch_d, sidebar, parent)
{
    swtch_d.buttonByWindowArea.insert(sidebar, this);

    HolonSidebarDockWidget *dock = desktop_d.sidebarDockWidget(sidebar);

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

HolonWindowAreaButton::HolonWindowAreaButton(HolonWindowAreaSwitchPrivate &swtch_d,
                                             HolonWindowArea *windowArea,
                                             HolonWindowAreaSwitch *parent)
:   HolonSwitchButton(swtch_d, windowArea, parent)
{
    swtch_d.buttonByWindowArea.insert(windowArea, this);

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
            desktop_d.q_ptr->setWindowArea(windowArea);

            for (QAbstractButton *button : switch_d.windowAreaButtonGroup->buttons())
            {
                if (button != this && button->isChecked())
                    button->setChecked(false);
            }
        }
        else if (!switch_d.windowAreaButtonGroup->checkedButton())
            desktop_d.q_ptr->setWindowArea(nullptr);
    });
}
