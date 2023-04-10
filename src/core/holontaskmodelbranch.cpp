// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskmodelbranch.h"
#include "holondesktop.h"
#include "holontaskmodel.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

class HolonTaskModelBranchPrivate
{
public:
    HolonTaskModel *const model;
    HolonTaskModelBranch *const branch;
    bool expanded{};

    HolonTaskModelBranchPrivate(HolonTaskModel *m)
    :   model(m),
        branch(nullptr)
    { }

    HolonTaskModelBranchPrivate(HolonTaskModelBranch *b)
    :   model(nullptr),
        branch(b)
    { }

    HolonDesktop *desktop() const
    {
        return (model ? model->desktop() : branch->desktop());
    }
};

HolonTaskModelBranch::HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModel *taskTreeModel)
:   QObject(taskTreeModel),
    QLoaderSettings(settings),
    d_ptr(new HolonTaskModelBranchPrivate(taskTreeModel))
{ }

HolonTaskModelBranch::HolonTaskModelBranch(QLoaderSettings *settings, HolonTaskModelBranch *taskTreeBranch)
:   QObject(taskTreeBranch),
    QLoaderSettings(settings),
    d_ptr(new HolonTaskModelBranchPrivate(taskTreeBranch))
{ }

HolonTaskModelBranch::~HolonTaskModelBranch()
{ }

HolonDesktop *HolonTaskModelBranch::desktop() const
{
    return d_ptr->desktop();
}

QIcon HolonTaskModelBranch::icon(Holon::TreeItem state) const
{
    if (state == Holon::Collapsed)
        return desktop()->currentTheme()->icons()->dirClosedIcon();

    return desktop()->currentTheme()->icons()->dirOpenIcon();
}

bool HolonTaskModelBranch::isExpanded() const
{
    return d_ptr->expanded;
}

void HolonTaskModelBranch::setExpanded(bool expanded)
{
    d_ptr->expanded = expanded;
}

HolonTaskModel *HolonTaskModelBranch::taskModel() const
{
    if (d_ptr->model)
        return d_ptr->model;

    return d_ptr->branch->taskModel();
}

QString HolonTaskModelBranch::title() const
{
    return value(u"title"_s, section().constLast()).toString();
}
