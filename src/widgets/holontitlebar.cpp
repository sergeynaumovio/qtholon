// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontitlebar.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holonid.h"
#include "holonsidebar.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonthemestyle.h"
#include "holonstackedwindow.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include <QActionGroup>
#include <QApplication>
#include <QBoxLayout>
#include <QComboBox>
#include <QEvent>
#include <QLabel>
#include <QLoaderTree>
#include <QMenu>
#include <QStyleOption>
#include <QStylePainter>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

class HolonTitleBarPrivate
{
public:
    HolonDesktop *const desktop;
    QComboBox *windowCombobox{};
    QToolButton *splitButton{};
    QToolButton *maximizeButton{};
    QToolButton *closeButton{};
    QToolButton *hideWindowAreaButton{};

    HolonTitleBarPrivate(HolonDesktop *desk)
    :   desktop(desk)
    { }

    void addWindow(HolonAbstractWindow *window)
    {
        int windowTypeId = window->metaObject()->metaType().id();

        for (int index{}; index < windowCombobox->count(); ++index)
        {
            QStringList section = windowCombobox->itemData(index).toStringList();
            int itemTypeId = window->tree()->object(section)->metaObject()->metaType().id();

            if (windowTypeId == itemTypeId)
            {
                // fix addToolbar(window);

                if (window->isCurrent())
                    windowCombobox->setCurrentIndex(index);
            }
        }
    }
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
    Holon::WindowType unknown{};
    Holon::WindowType type = (qobject_cast<HolonSidebar *>(window->parent()) ? Holon::SidebarWindow :
                              qobject_cast<HolonAbstractTask *>(window->parent()) ? Holon::TaskWindow : unknown);

    for (HolonAbstractWindow *second : window->desktop()->findChildren<HolonAbstractWindow *>(Qt::FindDirectChildrenOnly))
    {
        if (window->flags().testFlags(Holon::WindowSplitButtonHint) && type &&
            second->flags().testFlags(Holon::WindowSplitButtonHint | type))
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

        HolonThemeIcons *icons = desktop->theme()->icons();

        if (HolonStackedWindow *stacked = qobject_cast<HolonStackedWindow *>(window))
        {
            layout()->setContentsMargins({});

            QComboBox *&combobox = d_ptr->windowCombobox;
            combobox = new QComboBox(this);
            {
                QList<HolonAbstractWindow *> siblingWindowList = siblingWindows(window);
                for (const HolonAbstractWindow *siblingWindow : siblingWindowList)
                    combobox->addItem(siblingWindow->icon(), siblingWindow->title(), siblingWindow->section());

                combobox->setCurrentIndex(-1);

                connect(combobox, &QComboBox::currentIndexChanged, parent, [=](int index)
                {
                    QStringList section = combobox->itemData(index).toStringList();
                    int typeId = window->tree()->object(section)->metaObject()->metaType().id();
                    for (HolonAbstractWindow *child : window->findChildren<HolonAbstractWindow *>(Qt::FindDirectChildrenOnly))
                    {
                        if (typeId == child->metaObject()->metaType().id())
                        {
                            stacked->setWindow(child);
                            // fix setToolbar(window)

                            return;
                        }
                    }

                    QStringList to = window->section();
                    to.append(QString::number(HolonId::createChildId(window)));
                    window->tree()->copy(section, to);
                    HolonAbstractWindow *child = qobject_cast<HolonAbstractWindow *>(window->tree()->object(to));
                    stacked->setWindow(child);
                });

                layout()->addWidget(combobox);
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

            if (HolonSidebar *sidebar = qobject_cast<HolonSidebar *>(window->parent()))
            {
                Qt::Orientation orientation = sidebar->orientation();
                QList<HolonAbstractWindow *> siblingWindowList = siblingWindows(window);
                for (int index = 0; index < siblingWindowList.count(); ++index)
                {
                    HolonAbstractWindow *siblingWindow = siblingWindowList[index];
                    QAction *action = new QAction(siblingWindow->icon(), siblingWindow->title(), menu);
                    menu->addAction(action);
                    connect(action, &QAction::triggered, parent, [=]
                    {
                        windowarea_d_ptr->splitWindow(window, siblingWindow, orientation, index);
                    });
                }
            }
            else
            {
                QAction *split = new QAction(menu);
                split->setText(u"Split"_s);
                split->setIcon(icons->splitButtonVerticalMenuIcon());
                connect(split, &QAction::triggered, parent, [=]
                {
                    windowarea_d_ptr->splitWindow(window, Qt::Vertical);
                });

                QAction *splitSideBySide = new QAction(menu);
                splitSideBySide->setText(u"Split Side By Side"_s);
                splitSideBySide->setIcon(icons->splitButtonHorizontalMenuIcon());
                connect(splitSideBySide, &QAction::triggered, parent, [=]
                {
                    windowarea_d_ptr->splitWindow(window, Qt::Horizontal);
                });

                QAction *openNewWidnow = new QAction(menu);
                openNewWidnow->setText(u"Open in New Window"_s);
                openNewWidnow->setDisabled(true);

                menu->addActions({split, splitSideBySide, openNewWidnow});
            }

            d_ptr->splitButton = addButton(icons->splitButtonVerticalIcon());
            d_ptr->splitButton->show();
            d_ptr->splitButton->setMenu(menu);
            d_ptr->splitButton->setPopupMode(QToolButton::InstantPopup);
        }

        if (window->flags().testFlag(Holon::WindowMinMaxButtonsHint))
        {
            d_ptr->maximizeButton = addButton(icons->maximizeIcon());
            {
                connect(d_ptr->maximizeButton, &QToolButton::clicked, this, [=, this]()
                {
                    windowarea_d_ptr->maximized = !windowarea_d_ptr->maximized;
                    if (windowarea_d_ptr->maximized)
                    {
                        d_ptr->maximizeButton->setIcon(icons->minimizeIcon());
                        d_ptr->splitButton->setDisabled(true);
                        d_ptr->closeButton->setDisabled(true);
                    }
                    else
                    {
                        d_ptr->maximizeButton->setIcon(icons->maximizeIcon());
                        d_ptr->splitButton->setDisabled(false);
                        d_ptr->closeButton->setDisabled(false);
                    }

                    windowarea_d_ptr->maximizeWindow(parent);
                });
            }
        }

        Qt::DockWidgetArea area = windowarea_d_ptr->area();
        QIcon icon = area == Qt::LeftDockWidgetArea ? icons->splitButtonCloseLeftIcon() :
                     area == Qt::RightDockWidgetArea ? icons->splitButtonCloseRightIcon() :
                     area == Qt::TopDockWidgetArea ? icons->splitButtonCloseTopIcon() :
                                                     icons->splitButtonCloseBottomIcon();

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

void HolonTitleBar::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
}

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

    HolonThemeIcons *icons = d_ptr->desktop->theme()->icons();

    if (area == Qt::LeftDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseLeftIcon());

    if (area == Qt::RightDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseRightIcon());

    if (area == Qt::TopDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseTopIcon());

    if (area == Qt::BottomDockWidgetArea)
        return d_ptr->closeButton->setIcon(icons->splitButtonCloseBottomIcon());
}

void HolonTitleBar::setWindowComboboxIndex(int index)
{
    d_ptr->windowCombobox->setCurrentIndex(index);
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
