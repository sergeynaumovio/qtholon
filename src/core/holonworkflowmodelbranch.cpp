// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodelbranch.h"
#include "holondesktop.h"
#include "holontheme.h"
#include "holonthemeicons.h"
#include "holonworkflowitem.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch_p.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

HolonWorkflowModelBranch::HolonWorkflowModelBranch(QLoaderSettings *settings, HolonWorkflowModel *workflowModel)
:   QObject(workflowModel),
    QLoaderSettings(this, settings),
    d_ptr(new HolonWorkflowModelBranchPrivate(this, workflowModel))
{
    workflowModel->addBranch(this);

    if (!d_ptr->restoreTreeState())
        emitWarning(u"task tree state format not valid"_s);
}

HolonWorkflowModelBranch::~HolonWorkflowModelBranch()
{ }

void HolonWorkflowModelBranch::addTask(HolonAbstractTask *task)
{
    d_ptr->addTask(task);
}

QIcon HolonWorkflowModelBranch::icon(Holon::TreeItem state) const
{
    if (state == Holon::Collapsed)
        return workflowModel()->desktop()->currentTheme()->icons()->dirClosedIcon();

    return workflowModel()->desktop()->currentTheme()->icons()->dirOpenIcon();
}

bool HolonWorkflowModelBranch::isCurrent() const
{
    return value(u"current"_s).toBool();
}

bool HolonWorkflowModelBranch::isExpanded() const
{
    return d_ptr->expanded;
}

void HolonWorkflowModelBranch::setCurrent(bool current)
{
    setValue(u"current"_s, current);
}

void HolonWorkflowModelBranch::setExpanded(bool expanded)
{
    d_ptr->expanded = expanded;
}

QString HolonWorkflowModelBranch::title() const
{
    return value(u"title"_s, section().constLast()).toString();
}

HolonWorkflowModel *HolonWorkflowModelBranch::workflowModel() const
{
    return d_ptr->workflowModel;
}
