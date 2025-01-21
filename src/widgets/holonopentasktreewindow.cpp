// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonopentasktreewindow.h"
#include "holondesktop.h"
#include "holonid.h"
#include "holonsidebar.h"
#include "holonopentasktreeview.h"
#include "holonstackedwindow.h"
#include <QBoxLayout>
#include <QHeaderView>
#include <QIcon>
#include <QLabel>
#include <QLoaderTree>
#include <QTreeView>
#include <QUuid>

using namespace Qt::Literals::StringLiterals;

class HolonOpenTaskTreeWindowPrivate
{
public:
    HolonOpenTaskTreeWindow *const q_ptr;
    QLoaderSettings *const settings;
    HolonDesktop *const desktop;
    QTreeView *view{};


    HolonOpenTaskTreeWindowPrivate(HolonOpenTaskTreeWindow *q = nullptr,
                                   QLoaderSettings *s = nullptr,
                                   HolonDesktop *desk = nullptr)
    :   q_ptr(q),
        settings(s),
        desktop(desk)
    { }

    QWidget *widget()
    {
        if (!q_ptr)
            return nullptr;

        if (view)
            return view;

        return view = new HolonOpenTaskTreeView(desktop);
    }
};

HolonOpenTaskTreeWindow::HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent)
:   HolonAbstractWindow(settings, parent)
{
    parent->addWindow(this);
}

HolonOpenTaskTreeWindow::HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonOpenTaskTreeWindow::HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonStackedWindow *parent)
:   HolonAbstractWindow(settings, parent),
    d_ptr(this, settings, parent->desktop())
{
    parent->addWindow(this);
}

HolonOpenTaskTreeWindow::~HolonOpenTaskTreeWindow()
{ }

QWidget *HolonOpenTaskTreeWindow::centralWidget()
{
    return d_ptr->widget();
}

QIcon HolonOpenTaskTreeWindow::icon() const
{
    return {};
}

bool HolonOpenTaskTreeWindow::isCopyable(QStringView to) const
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

QString HolonOpenTaskTreeWindow::title() const
{
    return u"Open Tasks"_s;
}

QWidget *HolonOpenTaskTreeWindow::toolbarWidget()
{
    return {};
}
