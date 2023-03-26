// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontitlebar.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include <QActionGroup>
#include <QBoxLayout>
#include <QComboBox>
#include <QEvent>
#include <QLabel>
#include <QMenu>
#include <QPainter>
#include <QPushButton>
#include <QStyleOption>

using namespace Qt::Literals::StringLiterals;

class HolonTitleBarPrivate
{
public:
    QPushButton *splitButton{};
    QPushButton *maximizeButton{};
    QPushButton *closeButton{};
    QPushButton *hideWindowAreaButton{};
};

class MenuEventFilter : public QObject
{
    bool eventFilter(QObject *watched, QEvent *event) override
    {
        if (event->type() == QEvent::MouseButtonRelease)
            if (QMenu *menu = qobject_cast<QMenu *>(watched))
                if (QAction *action = menu->activeAction())
                    if (action->isCheckable())
                    {
                        action->trigger();
                        return true;
                    }

        return QObject::eventFilter(watched, event);
    }

public:
    MenuEventFilter(QObject *parent) : QObject(parent) { }
};

QList<HolonAbstractWindow *> HolonTitleBar::siblingWindows(HolonAbstractWindow *window)
{
    QList<HolonAbstractWindow *> windowList;

    for (HolonAbstractWindow *second : window->desktop()->windows())
    {
        Holon::WindowFlags taskWindow = Holon::TaskWindow | Holon::WindowSplitButtonHint;
        Holon::WindowFlags sidebarWindow = Holon::SidebarWindow | Holon::WindowSplitButtonHint;

        if ((window->flags().testFlags(taskWindow) && second->flags().testFlags(taskWindow)) ||
            (window->flags().testFlags(sidebarWindow) && second->flags().testFlags(sidebarWindow)))
        {
            windowList.append(second);
        }
    }

    return windowList;
}

void HolonTitleBar::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

HolonTitleBar::HolonTitleBar(HolonDesktop *desktop,
                             HolonDockWidget *parent,
                             HolonAbstractWindow *window,
                             HolonWindowAreaPrivate *windowarea_d_ptr)
:   QWidget(parent)
{
    setStyleSheet(desktop->titleBarStyleSheet());

    setLayout(new QHBoxLayout(this));
    {
        layout()->setContentsMargins({7, 0, 0, 0});
        layout()->setSpacing(0);

        if (window->flags().testFlag(Holon::WindowSplitButtonHint))
        {
            QComboBox *combobox = new QComboBox(this);
            {
                combobox->setFixedHeight(desktop->titleBarHeight());

                QList<HolonAbstractWindow *> siblingWindowList = siblingWindows(window);
                for (const HolonAbstractWindow *siblingWindow : siblingWindowList)
                    combobox->addItem(siblingWindow->icon(), siblingWindow->title());

                layout()->addWidget(combobox);
                combobox->setCurrentText(window->title());
            }
        }
        else
        {
            QLabel *label = new QLabel(window->title(), this);
            {
                label->setFixedHeight(desktop->titleBarHeight());
                layout()->addWidget(label);
            }
        }

        auto addButton = [=, this](const QChar &chr)
        {
            QPushButton *button = new QPushButton(chr, this);
            {
                button->hide();
                button->setFixedHeight(desktop->titleBarHeight());
                button->setFixedWidth(button->height() * 1.2);
                button->setFlat(true);
                button->setStyleSheet(desktop->buttonStyleSheet());
                layout()->addWidget(button);
            }
            return button;
        };

        auto addButtonIcon = [=, this](const QIcon &icon)
        {
            QPushButton *button = new QPushButton(icon, QString(), this);
            {
                button->hide();
                button->setFixedHeight(desktop->titleBarHeight());
                button->setFixedWidth(button->height() * 1.2);
                button->setFlat(true);
                button->setStyleSheet(desktop->buttonStyleSheet());
                layout()->addWidget(button);
            }
            return button;
        };

        if (window->flags().testFlag(Holon::WindowSplitButtonHint))
        {
            QMenu *menu = new QMenu(parent);

            QAction *split = new QAction(menu);
            split->setText(u"Split"_s);
            split->setCheckable(true);
            split->setChecked(true);

            QAction *splitSideBySide = new QAction(menu);
            splitSideBySide->setText(u"Split Side By Side"_s);
            splitSideBySide->setCheckable(true);

            QAction *openNewWidnow = new QAction(menu);
            openNewWidnow->setText(u"Open in New Window"_s);
            openNewWidnow->setCheckable(true);
            openNewWidnow->setDisabled(true);

            QActionGroup *group = new QActionGroup(menu);
            group->addAction(split);
            group->addAction(splitSideBySide);
            group->addAction(openNewWidnow);

            menu->addActions({split, splitSideBySide, openNewWidnow});
            menu->installEventFilter(new MenuEventFilter(menu));

            menu->addSeparator();

            QList<HolonAbstractWindow *> siblingWindowList = siblingWindows(window);
            for (HolonAbstractWindow *siblingWindow : siblingWindowList)
            {
                QAction *action = new QAction(siblingWindow->icon(), siblingWindow->title(), menu);
                menu->addAction(action);
                connect(action, &QAction::triggered, parent, [=]
                {
                    Qt::Orientation orientation = (split->isChecked() ? Qt::Vertical : Qt::Horizontal);
                    windowarea_d_ptr->splitWindow(window, siblingWindow, orientation);
                });
            }

            d_ptr->splitButton = addButtonIcon(desktop->currentTheme()->icons()->splitButtonHorizontalIcon());
            d_ptr->splitButton->show();
            d_ptr->splitButton->setMenu(menu);
        }

        if (window->flags().testFlag(Holon::WindowMinMaxButtonsHint))
        {
            d_ptr->maximizeButton = addButton(u'M');
            {
                connect(d_ptr->maximizeButton, &QPushButton::clicked, this, [=, this]()
                {
                    windowarea_d_ptr->maximized = !windowarea_d_ptr->maximized;
                    if (windowarea_d_ptr->maximized)
                    {
                        d_ptr->maximizeButton->setText(u"m"_s);
                        d_ptr->splitButton->hide();
                        d_ptr->closeButton->hide();
                    }
                    else
                    {
                        d_ptr->maximizeButton->setText(u"M"_s);
                        d_ptr->splitButton->show();
                        d_ptr->closeButton->show();
                    }

                    windowarea_d_ptr->maximizeWindow(parent);
                });
            }
        }

        if (window->flags().testAnyFlag(Holon::WindowCloseButtonHint))
        {
            d_ptr->closeButton = addButton(u'X');
            {
                connect(d_ptr->closeButton, &QPushButton::clicked, this, [=](){ desktop->closeWindow(window); });
            }
        }

        if (qobject_cast<HolonWindowArea *>(window->parent()))
        {
            Qt::DockWidgetArea area = windowarea_d_ptr->area();

            QChar ch = area == Qt::LeftDockWidgetArea ? u'L' :
                       area == Qt::RightDockWidgetArea ? u'R' :
                       area == Qt::TopDockWidgetArea ? u'T' : u'B';

            if (ch == 'B'_L1)
                d_ptr->hideWindowAreaButton = addButtonIcon(desktop->
                                                            currentTheme()->
                                                            icons()->splitButtonCloseBottomIcon());
            else
                d_ptr->hideWindowAreaButton = addButton(ch);
            {
                d_ptr->hideWindowAreaButton->show();

                connect(d_ptr->hideWindowAreaButton, &QPushButton::clicked, this, [=]()
                {
                    desktop->hideWindowArea(windowarea_d_ptr->q_ptr);
                });
            }
        }
    }
}

HolonTitleBar::~HolonTitleBar()
{ }

void HolonTitleBar::hideControlButtons()
{
    if (d_ptr->maximizeButton)
        d_ptr->maximizeButton->hide();

    if (d_ptr->closeButton)
        d_ptr->closeButton->hide();
}

void HolonTitleBar::hideSidebarButton()
{
    if (d_ptr->hideWindowAreaButton)
        d_ptr->hideWindowAreaButton->hide();
}

void HolonTitleBar::hideSplitButton()
{
    if (d_ptr->splitButton)
        d_ptr->splitButton->hide();
}

void HolonTitleBar::setDockWidgetArea(Qt::DockWidgetArea area)
{
    if (!d_ptr->closeButton)
        return;

    if (area == Qt::LeftDockWidgetArea)
        return d_ptr->closeButton->setText(u"L"_s);

    if (area == Qt::RightDockWidgetArea)
        return d_ptr->closeButton->setText(u"R"_s);

    if (area == Qt::TopDockWidgetArea)
        return d_ptr->closeButton->setText(u"T"_s);

    if (area == Qt::BottomDockWidgetArea)
        return d_ptr->closeButton->setText(u"B"_s);
}

void HolonTitleBar::showControlButtons()
{
    if (d_ptr->maximizeButton)
        d_ptr->maximizeButton->show();

    if (d_ptr->closeButton)
        d_ptr->closeButton->show();
}

void HolonTitleBar::showSidebarButton()
{
    if (d_ptr->hideWindowAreaButton)
        d_ptr->hideWindowAreaButton->show();
}

void HolonTitleBar::showSplitButton()
{
    if (d_ptr->splitButton)
        d_ptr->splitButton->show();
}
