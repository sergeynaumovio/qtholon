// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmessageswindow.h"
#include "holondesktop.h"
#include "holonsidebar.h"
#include "holonstackedwidget.h"
#include <QIcon>
#include <QLoaderTree>

class HolonMessagesWindowPrivate
{
public:
    HolonMessagesWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderTree *const tree;
    HolonDesktop *const desktop;
    HolonWorkflowModel *workflowModel{};
    HolonTaskStackedWidget *stackedWidget{};


    HolonMessagesWindowPrivate(HolonMessagesWindow *q, QLoaderSettings *s, HolonDesktop *desk)
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

HolonMessagesWindow::HolonMessagesWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonMessagesWindow::HolonMessagesWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonMessagesWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonMessagesWindow::~HolonMessagesWindow()
{ }

HolonAbstractWindow::Areas HolonMessagesWindow::areas() const
{
    return HolonAbstractWindow::Sidebar | HolonAbstractWindow::areas();
}

HolonAbstractWindow::Attributes HolonMessagesWindow::attributes() const
{
    return {};
}

QIcon HolonMessagesWindow::icon() const
{
    return {};
}

bool HolonMessagesWindow::isCopyable(const QStringList &to) const
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

QString HolonMessagesWindow::title() const
{
    return "Messages";
}

QWidget *HolonMessagesWindow::toolbar() const
{
    return {};
}

QWidget *HolonMessagesWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}
