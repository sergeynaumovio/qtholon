// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask_p.h"
#include "holonabstracttask.h"
#include "holontaskfolder.h"
#include "holonworkflowmodel.h"
#include "holonworkflowmodelbranch.h"
#include <QApplication>
#include <QMap>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q,
                                                   HolonTaskFolder *f)
:   q_ptr(q),
    folder(f),
    workflowModelBranch(nullptr),
    desktop(folder->desktop())
{ }

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q,
                                                   HolonWorkflowModelBranch *branch)
:   q_ptr(q),
    folder(nullptr),
    workflowModelBranch(branch),
    desktop(workflowModelBranch->workflowModel()->desktop())
{ }

void HolonAbstractTaskPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
