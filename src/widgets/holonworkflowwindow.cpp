// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowwindow.h"
#include "holondesktop.h"
#include "holonwindowarea.h"
#include "holonworkflowscene.h"
#include <QBoxLayout>
#include <QGraphicsView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QScrollBar>

using namespace Qt::Literals::StringLiterals;

class HolonWorkflowWindowPrivate
{
public:
    HolonWorkflowWindow *const q_ptr;
    HolonWorkflowScene *scene;
    QGraphicsView *view{};

    HolonWorkflowWindowPrivate(HolonWorkflowWindow *q = nullptr)
    :   q_ptr(q)
    { }

    QWidget *widget()
    {
        if (!q_ptr)
            return nullptr;

        if (view)
            return view;

        scene = new HolonWorkflowScene(q_ptr);
        view = new QGraphicsView(scene);
        view->setFrameStyle(QFrame::NoFrame);
        view->setAlignment(Qt::AlignLeft | Qt::AlignTop);

        return view;
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
    return Holon::WindowAllButtonsHint;
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

int HolonWorkflowWindow::role() const
{
    return Holon::SettingsRole;
}

QString HolonWorkflowWindow::title() const
{
    return u"Workflow"_s;
}

QWidget *HolonWorkflowWindow::toolbar() const
{
    return {};
}

QWidget *HolonWorkflowWindow::widget(int widgetRole) const
{
    if (widgetRole == Holon::NoRole)
        return d_ptr->widget();

    if (widgetRole == role())
        return new QLabel(u"Workflow Settings"_s);

    return {};
}
