// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonwindowareaswitch.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardockwidget.h"
#include "holontaskbar.h"
#include "holontheme.h"
#include "holonthemesizehints.h"
#include "holonwindowarea.h"
#include <QAbstractButton>
#include <QBoxLayout>
#include <QButtonGroup>
#include <QLabel>
#include <QMouseEvent>
#include <QPainter>
#include <QShortcut>

using namespace Qt::Literals::StringLiterals;

class HolonWindowAreaSwitchPrivate
{
public:
    HolonDesktopPrivate &desktop_d;

    QMap<QString, QButtonGroup *> sidebarButtonGroupByName;
    QButtonGroup *windowAreaButtonGroup{};
    QMap<HolonWindowArea *, QAbstractButton *> buttonByWindowArea;

    HolonWindowAreaSwitchPrivate(HolonDesktopPrivate &desk_d)
    :   desktop_d(desk_d)
    { }

    void closeWindowArea(HolonWindowArea *windowArea)
    {
        if (QAbstractButton *button = buttonByWindowArea.value(windowArea))
            button->setChecked(false);
    }
};

class HolonSwitchButton : public QAbstractButton
{
    float fontSize;

    void adjustSize()
    {
        HolonThemeSizeHints *sizeHints = desktop_d.q_ptr->currentTheme()->sizeHints();
        fontSize = sizeHints->taskbarSizeHint().height() / 2.6;

        if (desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Top ||
            desktop_d.taskbarArea() == HolonDesktopPrivate::TaskbarArea::Bottom)
        {
            setFixedHeight(sizeHints->taskbarSizeHint().height());
            setFixedWidth(sizeHints->sidebarSwitchButtonSizeHint().width());
        }
        else
        {
            setFixedWidth(sizeHints->taskbarSizeHint().width());
            setFixedHeight(sizeHints->taskbarSizeHint().width());
        }
    }

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
        case QEvent::Polish:
            adjustSize();
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

        QFont font(u"Arial"_s, fontSize);
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
        setAttribute(Qt::WA_Hover);
        setCheckable(true);
        setChecked(windowArea->isChecked());
        setMinimumSize(1, 1);

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
};

class HolonWindowAreaButton : public HolonSwitchButton
{
public:
    HolonWindowAreaButton(HolonWindowAreaSwitchPrivate &swtch_d,
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
