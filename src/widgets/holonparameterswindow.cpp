// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonparameterswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include "holonworkflowmodel.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QStackedWidget>
#include <QLoaderTree>
#include <QUuid>

class HolonParametersWindowPrivate
{
public:
    HolonParametersWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderTree *const tree;
    HolonDesktop *const desktop;
    HolonWorkflowModel *workflowModel{};
    HolonTaskStackedWidget *stackedWidget{};


    HolonParametersWindowPrivate(HolonParametersWindow *q, QLoaderSettings *s, HolonDesktop *desk)
    :   q_ptr(q),
        settings(s),
        tree(s->tree()),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (stackedWidget)
            return stackedWidget;

        stackedWidget = new HolonTaskStackedWidget(q_ptr->group());

        return stackedWidget;
    }
};

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonParametersWindow::HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonParametersWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonParametersWindow::~HolonParametersWindow()
{ }

HolonAbstractWindow::Area HolonParametersWindow::area() const
{
    return HolonAbstractWindow::Sidebar;
}

HolonAbstractWindow::Attributes HolonParametersWindow::attributes() const
{
    return {};
}

QIcon HolonParametersWindow::icon() const
{
    return {};
}

bool HolonParametersWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar*>(parent))
            return true;
    }

    return false;
}

QString HolonParametersWindow::title() const
{
    return "Parameters";
}

QWidget *HolonParametersWindow::toolbar() const
{
    return {};
}

QWidget *HolonParametersWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}