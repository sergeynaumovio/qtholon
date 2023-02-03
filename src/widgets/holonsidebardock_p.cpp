// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebardock_p.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include <QBoxLayout>
#include <QStackedWidget>
#include <QLabel>

class HolonDesktopPrivate;
class HolonMainWindow;
class HolonSidebar;
class HolonSidebarDock;
class HolonSidebarDockPrivateData;
class QStackedWidget;
class QWidget;

class HolonSidebarDockTitleBar : public QWidget
{
public:
    HolonSidebarDockTitleBar(HolonDesktopPrivate &desktop_d, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop_d.titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            QLabel *label = new QLabel("", this);
            {
                label->setFixedHeight(desktop_d.titleBarHeight());
                layout()->addWidget(label);
            }
        }
    }
};

class HolonSidebarDockPrivateData
{
public:
    HolonDesktopPrivate &desktop_d;
    HolonSidebarDock *const q_ptr;
    HolonMainWindow *const mainWindow;

    struct
    {
        HolonSidebarDockTitleBar *visible;
        QWidget *hidden;

    } titlebar;

    QStackedWidget *stackedWidget;
    QWidget *currentWidget;

    HolonSidebarDockPrivateData(HolonDesktopPrivate &desk_d,
                                HolonSidebarDock *q,
                                HolonMainWindow *parent)
    :   desktop_d(desk_d),
        q_ptr(q),
        mainWindow(parent)
    { }
};

HolonSidebarDockPrivate::HolonSidebarDockPrivate(HolonDesktopPrivate &desktop_d,
                                                 HolonSidebarDock *q,
                                                 const QString &name,
                                                 HolonMainWindow *parent)
:   d(*new (&d_storage) HolonSidebarDockPrivateData(desktop_d, q, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarDockPrivateData));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarDockPrivateData));

    d.titlebar.hidden = new QWidget(q);
    d.stackedWidget = new QStackedWidget(q);

    QLabel *label = new QLabel(q);
    {
        label->setText(name);
        QFont font("Arial", 20, QFont::Bold);
        label->setFont(font);
        label->setAlignment(Qt::AlignCenter);
        d.stackedWidget->addWidget(label);
    }

    q->setFeatures(QDockWidget::DockWidgetMovable);
    q->setTitleBarWidget(d.titlebar.hidden);
    q->setWidget(d.stackedWidget);
    q->setObjectName(name);
}

HolonSidebar *HolonSidebarDockPrivate::currentSidebar() const
{
    return static_cast<HolonSidebar *>(d.stackedWidget->currentWidget());
}

HolonDesktopPrivate &HolonSidebarDockPrivate::desktop_d() const
{
    return d.desktop_d;
}

void HolonSidebarDockPrivate::showTitleBarWidget(bool show) const
{
    if (show)
    {
        d.currentWidget = d.stackedWidget->currentWidget();
        d.stackedWidget->setCurrentIndex(0);
        d.q_ptr->setTitleBarWidget(d.titlebar.visible = new HolonSidebarDockTitleBar(d.desktop_d, d.q_ptr));
        d.titlebar.hidden->deleteLater();
    }
    else
    {
        d.stackedWidget->setCurrentWidget(d.currentWidget);
        d.q_ptr->setTitleBarWidget(d.titlebar.hidden = new QWidget(d.q_ptr));
        d.titlebar.visible->deleteLater();
    }
}

QStackedWidget *HolonSidebarDockPrivate::stackedWidget() const
{
    return d.stackedWidget;
}
