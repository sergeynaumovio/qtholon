// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonsettingswindow.h"
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

class HolonSettingsWindowPrivate
{
public:
    HolonSettingsWindow *const q_ptr;
    QLoaderSettings *const settings;
    QLoaderTree *const tree;
    HolonDesktop *const desktop;
    HolonWorkflowModel *workflowModel{};
    HolonWindowAreaStackedWidget *stackedWidget{};


    HolonSettingsWindowPrivate(HolonSettingsWindow *q, QLoaderSettings *s, HolonDesktop *desk)
    :   q_ptr(q),
        settings(s),
        tree(s->tree()),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (stackedWidget)
            return stackedWidget;

        stackedWidget = new HolonWindowAreaStackedWidget(q_ptr->group());

        return stackedWidget;
    }
};

HolonSettingsWindow::HolonSettingsWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonSettingsWindow::HolonSettingsWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(new HolonSettingsWindowPrivate(this, settings, parent->desktop()))
{
    parent->addWindow(this);
}

HolonSettingsWindow::~HolonSettingsWindow()
{ }

HolonAbstractWindow::Area HolonSettingsWindow::area() const
{
    return HolonAbstractWindow::Sidebar;
}

HolonAbstractWindow::Attributes HolonSettingsWindow::attributes() const
{
    return {};
}

QIcon HolonSettingsWindow::icon() const
{
    return {};
}

bool HolonSettingsWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonSidebar *>(parent))
            return true;
    }

    return false;
}

QString HolonSettingsWindow::title() const
{
    return "Settings";
}

QWidget *HolonSettingsWindow::toolbar() const
{
    return {};
}

QWidget *HolonSettingsWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    return {};
}
