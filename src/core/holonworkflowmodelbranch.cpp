// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodel.h"
#include <QIcon>

HolonWorkflowModelBranch::HolonWorkflowModelBranch(QLoaderSettings *settings, HolonWorkflowModel *workflowModel)
:   QObject(workflowModel),
    QLoaderSettings(settings)
{ }

QIcon HolonWorkflowModelBranch::icon() const
{
    return QIcon(value("icon", ":/holon/holoniconlight.svg").toString());
}

bool HolonWorkflowModelBranch::isCurrent() const
{
    return value("current").toBool();
}

void HolonWorkflowModelBranch::setCurrent(bool current)
{
    setValue("current", current);
}

QString HolonWorkflowModelBranch::title() const
{
    return value("title", section().constLast()).toString();
}

HolonWorkflowModel *HolonWorkflowModelBranch::workflowModel() const
{
    return qobject_cast<HolonWorkflowModel *>(parent());
}
