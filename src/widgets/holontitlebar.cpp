// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontitlebar.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow.h"
#include "holondesktop.h"
#include "holondockwidget.h"
#include "holonid.h"
#include "holonparameterswindow.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include "holonstackedwindow.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonthemestyle.h"
#include "holonwindowarea.h"
#include "holonwindowarea_p.h"
#include "holonworkflowwindow.h"
#include <QActionGroup>
#include <QApplication>
#include <QBoxLayout>
#include <QComboBox>
#include <QEvent>
#include <QLabel>
#include <QLoaderTree>
#include <QMetaMethod>
#include <QMenu>
#include <QStyleOption>
#include <QStylePainter>
#include <QToolButton>

using namespace Qt::Literals::StringLiterals;

bool static canSplit(HolonAbstractWindow *window)
{
    int settingsTypeId = QMetaType::fromType<QLoaderSettings *>().id();
    int stackedWindowTypeId = QMetaType::fromType<HolonStackedWindow *>().id();

    for (int c{}; c < window->metaObject()->constructorCount(); ++c)
    {
        QMetaMethod ctor = window->metaObject()->constructor(c);
        if (ctor.parameterType(0) == settingsTypeId && ctor.parameterType(1) == stackedWindowTypeId)
            return true;
    }

    return false;
}

class HolonTitleBarPrivate
{
public:
    HolonTitleBar *const q_ptr;
    HolonDesktop *const desktop;
    HolonDockWidget *const parent;
    HolonAbstractWindow *const window;
    HolonWindowAreaPrivate *const windowarea_d_ptr;
    HolonThemeIcons *const icons;
    const bool isStackedSidebarWindow;
    const bool isTaskWindow;
    const bool isWorkflowWindow;
    QComboBox *windowCombobox{};
    HolonWindowStackedWidget *stackedToolbar{};
    QToolButton *splitButton{};
    QToolButton *maximizeButton{};
    QToolButton *closeButton{};
    QToolButton *hideWindowAreaButton{};

    HolonTitleBarPrivate(HolonTitleBar *q,
                         HolonDesktop *desk,
                         HolonDockWidget *prnt,
                         HolonAbstractWindow *w,
                         HolonWindowAreaPrivate *windowarea_p)
    :   q_ptr(q),
        desktop(desk),
        parent(prnt),
        window(w),
        windowarea_d_ptr(windowarea_p),
        icons(desktop->theme()->icons()),
        isStackedSidebarWindow(qobject_cast<HolonStackedWindow *>(window)),
        isTaskWindow(qobject_cast<HolonAbstractTask *>(window->parent())),
        isWorkflowWindow(qobject_cast<HolonWorkflowWindow *>(window))
    {
        q_ptr->setFixedHeight(QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));

        q_ptr->setLayout(new QHBoxLayout(q_ptr));
        {
            q_ptr->layout()->setSpacing(0);

            addCombobox();
            addStackedToolbar();
            addSplitButton();
            addMaximizeButton();
            addCloseButton();
            addHideWindowAreaButton();
        }
    }

    QToolButton *addButton(QIcon icon)
    {
        QToolButton *button = new QToolButton(q_ptr);
        button->hide();
        button->setFixedHeight(q_ptr->height());
        button->setFixedWidth(button->height() * 1.2);
        button->setIcon(icon);
        q_ptr->layout()->addWidget(button);

        return button;
    }

    void addCloseButton()
    {
        QIcon icon = areaIcon();
        closeButton = addButton(icon);
        {
            QObject::connect(closeButton, &QToolButton::clicked, q_ptr, [this](){ desktop->closeWindow(window); });
        }
    }

    void addCombobox()
    {
        if (HolonStackedWindow *stacked = qobject_cast<HolonStackedWindow *>(window))
        {
            q_ptr->layout()->setContentsMargins({});

            windowCombobox = new QComboBox(q_ptr);
            {
                QList<HolonAbstractWindow *> siblingWindowList = siblingWindows(window);
                for (const HolonAbstractWindow *siblingWindow : siblingWindowList)
                    windowCombobox->addItem(siblingWindow->icon(), siblingWindow->title(), siblingWindow->section());

                windowCombobox->setCurrentIndex(-1);

                QObject::connect(windowCombobox, &QComboBox::currentIndexChanged, parent, [=, this](int index)
                {
                    QStringList section = windowCombobox->itemData(index).toStringList();
                    QMetaType type = window->tree()->object(section)->metaObject()->metaType();
                    for (HolonAbstractWindow *child : window->findChildren<HolonAbstractWindow *>(Qt::FindDirectChildrenOnly))
                    {
                        if (type == child->metaObject()->metaType())
                        {
                            stacked->setWindow(child);
                            stackedToolbar->setCurrentWindow(child);

                            return;
                        }
                    }

                    QStringList to = window->section();
                    to.append(QString::number(HolonId::createChildId(window)));
                    window->tree()->copy(section, to);
                    HolonAbstractWindow *child = qobject_cast<HolonAbstractWindow *>(window->tree()->object(to));
                    stacked->setWindow(child);
                    stackedToolbar->setCurrentWindow(child);
                });

                q_ptr->layout()->addWidget(windowCombobox);
            }
        }
        else if (qobject_cast<HolonParametersWindow *>(window) || qobject_cast<HolonWorkflowWindow *>(window))
        {
            q_ptr->layout()->setContentsMargins({});

            windowCombobox = new QComboBox(q_ptr);
            q_ptr->layout()->addWidget(windowCombobox);
        }
        else
        {
            q_ptr->layout()->setContentsMargins(5, 0, 0, 0);
            q_ptr->layout()->addWidget(new QLabel(window->title(), q_ptr));
        }
    }

    void addHideWindowAreaButton()
    {
        QIcon icon = areaIcon();

        if (qobject_cast<HolonWindowArea *>(window->parent()))
        {
            hideWindowAreaButton = addButton(icon);
            {
                hideWindowAreaButton->show();

                QObject::connect(hideWindowAreaButton, &QToolButton::clicked, q_ptr, [this]()
                {
                    desktop->closeWindowArea(windowarea_d_ptr->q_ptr);
                });
            }
        }
    }

    void addMaximizeButton()
    {
        if (isStackedSidebarWindow || isTaskWindow || isWorkflowWindow)
        {
            maximizeButton = addButton(icons->maximizeIcon());
            {
                QObject::connect(maximizeButton, &QToolButton::clicked, q_ptr, [=, this]()
                {
                    windowarea_d_ptr->maximized = !windowarea_d_ptr->maximized;
                    if (windowarea_d_ptr->maximized)
                    {
                        maximizeButton->setIcon(icons->minimizeIcon());
                        splitButton->setDisabled(true);
                        closeButton->setDisabled(true);
                    }
                    else
                    {
                        maximizeButton->setIcon(icons->maximizeIcon());
                        splitButton->setDisabled(false);
                        closeButton->setDisabled(false);
                    }

                    windowarea_d_ptr->maximizeWindow(parent);
                });
            }
        }
    }

    void addSplitButton()
    {
        if (isStackedSidebarWindow || isTaskWindow || isWorkflowWindow)
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
                    QObject::connect(action, &QAction::triggered, parent, [=, this]
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
                QObject::connect(split, &QAction::triggered, parent, [this]
                {
                    windowarea_d_ptr->splitWindow(window, Qt::Vertical);
                });

                QAction *splitSideBySide = new QAction(menu);
                splitSideBySide->setText(u"Split Side By Side"_s);
                splitSideBySide->setIcon(icons->splitButtonHorizontalMenuIcon());
                QObject::connect(splitSideBySide, &QAction::triggered, parent, [this]
                {
                    windowarea_d_ptr->splitWindow(window, Qt::Horizontal);
                });

                QAction *openNewWidnow = new QAction(menu);
                openNewWidnow->setText(u"Open in New Window"_s);
                openNewWidnow->setDisabled(true);

                menu->addActions({split, splitSideBySide, openNewWidnow});
            }

            splitButton = addButton(icons->splitButtonVerticalIcon());
            splitButton->show();
            splitButton->setMenu(menu);
            splitButton->setPopupMode(QToolButton::InstantPopup);
        }
    }

    void addStackedToolbar()
    {
        stackedToolbar = new HolonWindowStackedWidget;
        stackedToolbar->setSizePolicy({QSizePolicy::Maximum, QSizePolicy::Maximum});
        q_ptr->layout()->addWidget(stackedToolbar);
    }

    void addWindow(HolonAbstractWindow *wnd)
    {
        if (QWidget *toolbarWidget = wnd->toolbarWidget())
            stackedToolbar->addWindowWidget(wnd, toolbarWidget);

        if (qobject_cast<HolonStackedWindow *>(wnd->parent()) && wnd->isCurrent())
        {
            QMetaType windowType = wnd->metaObject()->metaType();

            for (int index{}; index < windowCombobox->count(); ++index)
            {
                QStringList section = windowCombobox->itemData(index).toStringList();
                QMetaType itemType = wnd->tree()->object(section)->metaObject()->metaType();

                if (windowType == itemType)
                    return windowCombobox->setCurrentIndex(index);
            }
        }
    }

    QIcon areaIcon()
    {
        Qt::DockWidgetArea area = windowarea_d_ptr->area();

        QIcon icon = area == Qt::LeftDockWidgetArea ? icons->splitButtonCloseLeftIcon() :
                     area == Qt::RightDockWidgetArea ? icons->splitButtonCloseRightIcon() :
                     area == Qt::TopDockWidgetArea ? icons->splitButtonCloseTopIcon() :
                                                     icons->splitButtonCloseBottomIcon();
        return icon;
    }

    QList<HolonAbstractWindow *> siblingWindows(HolonAbstractWindow *wnd)
    {
        QList<HolonAbstractWindow *> windowList;

        if (qobject_cast<HolonAbstractTask *>(wnd->parent()))
        {
            for (HolonAbstractWindow *second : wnd->desktop()->findChildren<HolonAbstractTaskWindow *>(Qt::FindDirectChildrenOnly))
                windowList.append(second);
        }
        else if (qobject_cast<HolonSidebar *>(wnd->parent()))
        {
            for (HolonAbstractWindow *second : wnd->desktop()->findChildren<HolonAbstractWindow *>(Qt::FindDirectChildrenOnly))
                if (canSplit(second))
                    windowList.append(second);
        }

        return windowList;
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
    d_ptr(this, desktop, parent, window, windowarea_d_ptr)
{ }

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

QComboBox *HolonTitleBar::windowComboBox() const
{
    return d_ptr->windowCombobox;
}
