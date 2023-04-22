// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontitlebar.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonthemestyle.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include <QActionGroup>
#include <QApplication>
#include <QBoxLayout>
#include <QComboBox>
#include <QEvent>
#include <QLabel>
#include <QMenu>
#include <QStyleOption>
#include <QStylePainter>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

class HolonTitleBarPrivate
{
public:
    HolonDesktop *const desktop;
    QToolButton *splitButton{};
    QToolButton *maximizeButton{};
    QToolButton *closeButton{};
    QToolButton *hideWindowAreaButton{};

    HolonTitleBarPrivate(HolonDesktop *desk)
    :   desktop(desk)
    { }
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
    QStyle::PrimitiveElement pe = static_cast<QStyle::PrimitiveElement>(HolonThemeStyle::PE_TitleBar);
    QStyleOption option;
    option.initFrom(this);
    QStylePainter(this).drawPrimitive(pe, option);
}

HolonTitleBar::HolonTitleBar(HolonDesktop *desktop,
                             HolonDockWidget *parent,
                             HolonAbstractWindow *window,
                             HolonWindowAreaPrivate *windowarea_d_ptr)
:   QWidget(parent),
    d_ptr(desktop)
{
    setFixedHeight(QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));

    setLayout(new QHBoxLayout(this));
    {
        layout()->setSpacing(0);

        if (window->flags().testFlag(Holon::WindowSplitButtonHint))
        {
            layout()->setContentsMargins({});
            QComboBox *combobox = new QComboBox(this);
            {
                QList<HolonAbstractWindow *> siblingWindowList = siblingWindows(window);
                for (const HolonAbstractWindow *siblingWindow : siblingWindowList)
                    combobox->addItem(siblingWindow->icon(), siblingWindow->title());

                layout()->addWidget(combobox);
                combobox->setCurrentText(window->title());
            }
        }
        else
        {
            layout()->setContentsMargins(5, 0, 0, 0);
            layout()->addWidget(new QLabel(window->title(), this));
        }

        auto addButton = [=, this](const QIcon &icon)
        {
            QToolButton *button = new QToolButton(this);
            {
                button->hide();
                button->setFixedHeight(height());
                button->setFixedWidth(button->height() * 1.2);
                button->setIcon(icon);
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

            d_ptr->splitButton = addButton(desktop->currentTheme()->icons()->splitButtonHorizontalIcon());
            d_ptr->splitButton->show();
            d_ptr->splitButton->setMenu(menu);
            d_ptr->splitButton->setPopupMode(QToolButton::InstantPopup);
        }

        if (window->flags().testFlag(Holon::WindowMinMaxButtonsHint))
        {
            d_ptr->maximizeButton = addButton(desktop->currentTheme()->icons()->maximizeIcon());
            {
                connect(d_ptr->maximizeButton, &QToolButton::clicked, this, [=, this]()
                {
                    windowarea_d_ptr->maximized = !windowarea_d_ptr->maximized;
                    if (windowarea_d_ptr->maximized)
                    {
                        d_ptr->maximizeButton->setIcon(desktop->currentTheme()->icons()->minimizeIcon());
                        d_ptr->splitButton->setDisabled(true);
                        d_ptr->closeButton->setDisabled(true);
                    }
                    else
                    {
                        d_ptr->maximizeButton->setIcon(desktop->currentTheme()->icons()->maximizeIcon());
                        d_ptr->splitButton->setDisabled(false);
                        d_ptr->closeButton->setDisabled(false);
                    }

                    windowarea_d_ptr->maximizeWindow(parent);
                });
            }
        }

        Qt::DockWidgetArea area = windowarea_d_ptr->area();
        QIcon icon;

        if (area)
        {
            HolonThemeIcons *icons = desktop->currentTheme()->icons();
            icon = area == Qt::LeftDockWidgetArea ? icons->splitButtonCloseLeftIcon() :
                   area == Qt::RightDockWidgetArea ? icons->splitButtonCloseRightIcon() :
                   area == Qt::TopDockWidgetArea ? icons->splitButtonCloseTopIcon() :
                                                   icons->splitButtonCloseBottomIcon();
        }

        if (window->flags().testAnyFlag(Holon::WindowCloseButtonHint))
        {
            d_ptr->closeButton = addButton(icon);
            {
                connect(d_ptr->closeButton, &QToolButton::clicked, this, [=](){ desktop->closeWindow(window); });
            }
        }

        if (qobject_cast<HolonWindowArea *>(window->parent()))
        {
            d_ptr->hideWindowAreaButton = addButton(icon);
            {
                d_ptr->hideWindowAreaButton->show();

                connect(d_ptr->hideWindowAreaButton, &QToolButton::clicked, this, [=]()
                {
                    desktop->closeWindowArea(windowarea_d_ptr->q_ptr);
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

    HolonThemeIcons *icons = d_ptr->desktop->currentTheme()->icons();

    if (area == Qt::LeftDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseLeftIcon());

    if (area == Qt::RightDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseRightIcon());

    if (area == Qt::TopDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseTopIcon());

    if (area == Qt::BottomDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseBottomIcon());
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
