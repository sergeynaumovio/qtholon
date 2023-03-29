// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask_p.h"
#include "holonabstracttask.h"
#include "holonworkflowmodelbranch.h"
#include "holonworkflowmodel.h"
#include <QApplication>
#include <QMap>
#include <QStyle>

using namespace Qt::Literals::StringLiterals;

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q, HolonWorkflowModelBranch *branch)
:   q_ptr(q),
    workflowModelBranch(branch),
    desktop([this]() -> HolonDesktop *
    {
        if (workflowModelBranch)
            if (HolonWorkflowModel *workflowModel = workflowModelBranch->workflowModel())
                return workflowModel->desktop();

        return {};
    }())
{ }

QIcon HolonAbstractTaskPrivate::icon()
{
    if (ico.isNull())
        ico.addPixmap(QApplication::style()->standardIcon(QStyle::SP_FileIcon).pixmap(QSize(16, 16)));

    return ico;
}

void HolonAbstractTaskPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
