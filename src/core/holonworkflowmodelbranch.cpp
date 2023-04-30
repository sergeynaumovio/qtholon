// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodel.h"
#include <QIcon>

using namespace Qt::Literals::StringLiterals;

HolonWorkflowModelBranch::HolonWorkflowModelBranch(QLoaderSettings *settings, HolonWorkflowModel *workflowModel)
:   QObject(workflowModel),
    QLoaderSettings(this, settings)
{ }

QIcon HolonWorkflowModelBranch::icon() const
{
    return QIcon(value(u"icon"_s, u":/holon/holoniconlight.svg"_s).toString());
}

bool HolonWorkflowModelBranch::isCurrent() const
{
    return value(u"current"_s).toBool();
}

void HolonWorkflowModelBranch::setCurrent(bool current)
{
    setValue(u"current"_s, current);
}

QString HolonWorkflowModelBranch::title() const
{
    return value(u"title"_s, section().constLast()).toString();
}

HolonWorkflowModel *HolonWorkflowModelBranch::workflowModel() const
{
    return qobject_cast<HolonWorkflowModel *>(parent());
}
