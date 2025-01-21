// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonprojecttasktreewindow.h"
#include "holonabstracttask.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holonprojecttasktreewindow_p.h"
#include "holonsidebar.h"
#include "holonstackedwindow.h"
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

HolonProjectTaskTreeWindow::HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonStackedWindow *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonProjectTaskTreeWindow::~HolonProjectTaskTreeWindow()
{ }

QWidget *HolonProjectTaskTreeWindow::centralWidget()
{
    return d_ptr->widget();
}

QIcon HolonProjectTaskTreeWindow::icon() const
{
    return {};
}

bool HolonProjectTaskTreeWindow::isCopyable(QStringView to) const
{
    QStringView toParent = HolonId::parentSection(to);

    if (toParent.size())
    {
        if (QObject *parent = tree()->object(toParent))
            if (qobject_cast<HolonSidebar *>(parent) || qobject_cast<HolonStackedWindow *>(parent))
                return true;
    }

    return false;
}

QString HolonProjectTaskTreeWindow::title() const
{
    return u"Project Tasks"_s;
}

QWidget *HolonProjectTaskTreeWindow::toolbarWidget()
{
    return {};
}
