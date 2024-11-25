// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonprojecttasktreewindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonprojecttasktreewindow_p.h"
#include "holonsidebar.h"
#include "holontaskfolder.h"
#include "holontaskfoldermodel.h"
#include "holonworkflow.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QTreeView>

using namespace Qt::Literals::StringLiterals;

HolonProjectTaskTreeWindow::HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonProjectTaskTreeWindow::HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonProjectTaskTreeWindow::~HolonProjectTaskTreeWindow()
{ }

Holon::WindowFlags HolonProjectTaskTreeWindow::flags() const
{
    return Holon::SidebarWindow | Holon::WindowAllButtonsHint;
}

QIcon HolonProjectTaskTreeWindow::icon() const
{
    return {};
}

bool HolonProjectTaskTreeWindow::isCopyable(const QStringList &to) const
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

QString HolonProjectTaskTreeWindow::title() const
{
    return u"Project Tasks"_s;
}

QWidget *HolonProjectTaskTreeWindow::toolbar() const
{
    return {};
}

QWidget *HolonProjectTaskTreeWindow::widget(int widgetRole) const
{
    if (widgetRole == Holon::NoRole)
        return d_ptr->widget();

    return {};
}
