// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebardock_p.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardock.h"
#include "holontheme.h"
#include "holonthemestylesheets.h"
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

using namespace Qt::Literals::StringLiterals;

class HolonSidebarDockTitleBar : public QWidget
{
    const int titleBarHeight{24};

public:
    HolonSidebarDockTitleBar(HolonDesktopPrivate &desktop_d, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop_d.q_ptr->currentTheme()->styleSheets()->titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            layout()->setContentsMargins({});
            QLabel *label = new QLabel(u""_s, this);
            {
                label->setFixedHeight(titleBarHeight);
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
                QFont font(u"Arial"_s, 20, QFont::Bold);
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
:   d_ptr(desktop_d, q, name, parent)
{
    q->setFeatures(QDockWidget::DockWidgetMovable);
    q->setTitleBarWidget(d_ptr->titlebar.hidden);
    q->setWidget(d_ptr->stackedWidget.root);
    q->setObjectName(name);
}

HolonSidebarDockPrivate::~HolonSidebarDockPrivate()
{ }

void HolonSidebarDockPrivate::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->stackedWidget.addSidebar(sidebar);
}

HolonSidebar *HolonSidebarDockPrivate::currentSidebar() const
{
    return static_cast<HolonSidebar *>(d_ptr->stackedWidget.sidebars.stacked->currentWidget());
}

HolonDesktopPrivate &HolonSidebarDockPrivate::desktop_d() const
{
    return d_ptr->desktop_d;
}

void HolonSidebarDockPrivate::setSidebar(HolonSidebar *sidebar)
{
    d_ptr->stackedWidget.setSidebar(sidebar);
}

const QList<HolonSidebar *> &HolonSidebarDockPrivate::sidebars() const
{
    return d_ptr->stackedWidget.sidebars.list;
}

void HolonSidebarDockPrivate::showTitleBarWidget(bool show) const
{
    if (show)
    {
        d_ptr->stackedWidget.setLabel();
        d_ptr->q_ptr->setTitleBarWidget(d_ptr->titlebar.visible = new HolonSidebarDockTitleBar(d_ptr->desktop_d, d_ptr->q_ptr));
        d_ptr->titlebar.hidden->deleteLater();
    }
    else
    {
        d_ptr->stackedWidget.setSidebars();
        d_ptr->q_ptr->setTitleBarWidget(d_ptr->titlebar.hidden = new QWidget(d_ptr->q_ptr));
        d_ptr->titlebar.visible->deleteLater();
    }
}
