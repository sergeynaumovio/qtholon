// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsidebardockwidget_p.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"
#include "holonsidebardockwidget.h"
#include "holontheme.h"
#include "holonthemestyle.h"
#include <QApplication>
#include <QBoxLayout>
#include <QLabel>
#include <QPainter>
#include <QStackedWidget>
#include <QStyleOption>

using namespace Qt::Literals::StringLiterals;

class HolonSidebarDockTitleBar : public QWidget
{
    void paintEvent(QPaintEvent *)
    {
        QStyle::PrimitiveElement pe = static_cast<QStyle::PrimitiveElement>(HolonThemeStyle::PE_TitleBar);
        QStyleOption opt;
        opt.initFrom(this);
        QPainter p(this);
        style()->drawPrimitive(pe, &opt, &p, this);
    }

public:
    HolonSidebarDockTitleBar(QDockWidget *parent)
    :   QWidget(parent)
    {
        setFixedHeight(QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));
        setLayout(new QHBoxLayout(this));
    }
};

class HolonSidebarDockWidgetPrivateData
{
public:
    HolonDesktopPrivate &desktop_d;
    HolonSidebarDockWidget *const q_ptr;
    HolonSidebarMainWindow *const sidebarMainWindow;

    struct Titlebar
    {
        HolonSidebarDockTitleBar *visible;
        QWidget *hidden;

        Titlebar(HolonSidebarDockWidget *q)
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

        StackedWidget(HolonSidebarDockWidget *q, const QString &name)
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

    HolonSidebarDockWidgetPrivateData(HolonDesktopPrivate &desk_d,
                                      HolonSidebarDockWidget *q,
                                      const QString &name,
                                      HolonSidebarMainWindow *parent)
    :   desktop_d(desk_d),
        q_ptr(q),
        sidebarMainWindow(parent),
        titlebar(q),
        stackedWidget(q, name)
    { }
};

HolonSidebarDockWidgetPrivate::HolonSidebarDockWidgetPrivate(HolonDesktopPrivate &desktop_d,
                                                             HolonSidebarDockWidget *q,
                                                             const QString &name,
                                                             HolonSidebarMainWindow *parent)
:   d_ptr(desktop_d, q, name, parent)
{
    q->setFeatures(QDockWidget::DockWidgetMovable);
    q->setTitleBarWidget(d_ptr->titlebar.hidden);
    q->setWidget(d_ptr->stackedWidget.root);
    q->setObjectName(name);
}

HolonSidebarDockWidgetPrivate::~HolonSidebarDockWidgetPrivate()
{ }

void HolonSidebarDockWidgetPrivate::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->stackedWidget.addSidebar(sidebar);
}

HolonSidebar *HolonSidebarDockWidgetPrivate::currentSidebar() const
{
    return static_cast<HolonSidebar *>(d_ptr->stackedWidget.sidebars.stacked->currentWidget());
}

HolonDesktopPrivate &HolonSidebarDockWidgetPrivate::desktop_d() const
{
    return d_ptr->desktop_d;
}

void HolonSidebarDockWidgetPrivate::setSidebar(HolonSidebar *sidebar)
{
    d_ptr->stackedWidget.setSidebar(sidebar);
}

const QList<HolonSidebar *> &HolonSidebarDockWidgetPrivate::sidebars() const
{
    return d_ptr->stackedWidget.sidebars.list;
}

void HolonSidebarDockWidgetPrivate::showTitleBarWidget(bool show) const
{
    if (show)
    {
        d_ptr->stackedWidget.setLabel();
        d_ptr->q_ptr->setTitleBarWidget(d_ptr->titlebar.visible = new HolonSidebarDockTitleBar(d_ptr->q_ptr));
        d_ptr->titlebar.hidden->deleteLater();
    }
    else
    {
        d_ptr->stackedWidget.setSidebars();
        d_ptr->q_ptr->setTitleBarWidget(d_ptr->titlebar.hidden = new QWidget(d_ptr->q_ptr));
        d_ptr->titlebar.visible->deleteLater();
    }
}

HolonSidebarMainWindow *HolonSidebarDockWidgetPrivate::sidebarMainWindow() const
{
    return d_ptr->sidebarMainWindow;
}
