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

    struct Titlebar
    {
        HolonSidebarDockTitleBar *visible;
        QWidget *hidden;

        Titlebar(HolonSidebarDock *q)
        :   hidden(new QWidget(q))
        { }

    } titlebar;

    struct StackedWidget
    {
        QLabel *const label;
        QStackedWidget *const root;

        struct Sidebars
        {
            QStackedWidget *const stacked;
            QList<HolonSidebar *> list;

            Sidebars(QStackedWidget *parent)
            :   stacked(new QStackedWidget(parent))
            { }

            void setSidebar(HolonSidebar *sidebar) { stacked->setCurrentWidget(sidebar); }

        } sidebars;

        StackedWidget(HolonSidebarDock *q, const QString &name)
        :   label(new QLabel(name, q)),
            root(new QStackedWidget(q)),
            sidebars(root)
        {
            root->addWidget(label);
            {
                QFont font("Arial", 20, QFont::Bold);
                label->setFont(font);
                label->setAlignment(Qt::AlignCenter);
            }

            root->addWidget(sidebars.stacked);
            {
                root->setCurrentWidget(sidebars.stacked);
            }
        }

        void addSidebar(HolonSidebar *sidebar)
        {
            sidebars.stacked->addWidget(sidebar);

            if (sidebar->isChecked())
                sidebars.stacked->setCurrentWidget(sidebar);

            sidebars.list.append(sidebar);
        }

        void setLabel() { root->setCurrentWidget(label); }
        void setSidebars() { root->setCurrentWidget(sidebars.stacked); }
        void setSidebar(HolonSidebar *sidebar) { sidebars.setSidebar(sidebar); }

    } stackedWidget;

    HolonSidebarDockPrivateData(HolonDesktopPrivate &desk_d,
                                HolonSidebarDock *q,
                                const QString &name,
                                HolonMainWindow *parent)
    :   desktop_d(desk_d),
        q_ptr(q),
        mainWindow(parent),
        titlebar(q),
        stackedWidget(q, name)
    { }
};

HolonSidebarDockPrivate::HolonSidebarDockPrivate(HolonDesktopPrivate &desktop_d,
                                                 HolonSidebarDock *q,
                                                 const QString &name,
                                                 HolonMainWindow *parent)
:   d(*new (&d_storage) HolonSidebarDockPrivateData(desktop_d, q, name, parent))
{
    static_assert (sizeof (d_storage) == sizeof (HolonSidebarDockPrivateData));
    static_assert (sizeof (ptrdiff_t) == alignof (HolonSidebarDockPrivateData));


    q->setFeatures(QDockWidget::DockWidgetMovable);
    q->setTitleBarWidget(d.titlebar.hidden);
    q->setWidget(d.stackedWidget.root);
    q->setObjectName(name);
}

void HolonSidebarDockPrivate::addSidebar(HolonSidebar *sidebar)
{
    d.stackedWidget.addSidebar(sidebar);
}

HolonSidebar *HolonSidebarDockPrivate::currentSidebar() const
{
    return static_cast<HolonSidebar *>(d.stackedWidget.sidebars.stacked->currentWidget());
}

HolonDesktopPrivate &HolonSidebarDockPrivate::desktop_d() const
{
    return d.desktop_d;
}

void HolonSidebarDockPrivate::setSidebar(HolonSidebar *sidebar)
{
    d.stackedWidget.setSidebar(sidebar);
}

const QList<HolonSidebar *> &HolonSidebarDockPrivate::sidebars() const
{
    return d.stackedWidget.sidebars.list;
}

void HolonSidebarDockPrivate::showTitleBarWidget(bool show) const
{
    if (show)
    {
        d.stackedWidget.setLabel();
        d.q_ptr->setTitleBarWidget(d.titlebar.visible = new HolonSidebarDockTitleBar(d.desktop_d, d.q_ptr));
        d.titlebar.hidden->deleteLater();
    }
    else
    {
        d.stackedWidget.setSidebars();
        d.q_ptr->setTitleBarWidget(d.titlebar.hidden = new QWidget(d.q_ptr));
        d.titlebar.visible->deleteLater();
    }
}
