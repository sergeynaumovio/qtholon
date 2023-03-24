// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowwindow.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include <QBoxLayout>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

class HolonWorkflowWindowPrivate
{
public:
    HolonWorkflowWindow *const q_ptr;
    QWidget *central{};

    HolonWorkflowWindowPrivate(HolonWorkflowWindow *q = nullptr)
    :   q_ptr(q)
    { }


    QWidget *widget()
    {
        if (!q_ptr)
            return nullptr;

        if (central)
            return central;

        central = new QWidget;
        central->setLayout(new QVBoxLayout(central));
        central->layout()->addWidget(new QLabel(u"Workflows"_s, central));

        return central;
    }
};

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::HolonWorkflowWindow(QLoaderSettings *settings, HolonWindowArea *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this)
{
    parent->addWindow(this);
}

HolonWorkflowWindow::~HolonWorkflowWindow()
{ }

Holon::WindowFlags HolonWorkflowWindow::flags() const
{
    return {};
}

QIcon HolonWorkflowWindow::icon() const
{
    return {};
}

bool HolonWorkflowWindow::isCopyable(const QStringList &to) const
{
    QStringList parentSection = to;
    if (to.size() > 1)
    {
        parentSection.removeLast();
        QObject *parent = tree()->object(parentSection);
        if (qobject_cast<HolonWindowArea *>(parent))
            return true;
    }

    return false;
}

QString HolonWorkflowWindow::title() const
{
    return u"Workflow"_s;
}

QWidget *HolonWorkflowWindow::toolbar() const
{
    return {};
}

QWidget *HolonWorkflowWindow::widget(const QString &group) const
{
    if (group == QString())
        return d_ptr->widget();

    if (group == HolonAbstractWindow::group())
        return new QLabel(u"Workflow Settings"_s);

    return {};
}
