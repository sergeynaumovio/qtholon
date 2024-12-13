// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstracttask_p.h"
#include "holonabstracttask.h"
#include "holontaskfolder.h"
#include "holonworkflow.h"
#include <QApplication>

using namespace Qt::Literals::StringLiterals;

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q,
                                                   HolonTaskFolder *f)
:   q_ptr(q),
    folder(f),
    workflow(nullptr),
    desktop(folder->desktop())
{ }

HolonAbstractTaskPrivate::HolonAbstractTaskPrivate(HolonAbstractTask *q,
                                                   HolonWorkflow *w)
:   q_ptr(q),
    folder(nullptr),
    workflow(w),
    desktop(w->desktop())
{ }

void HolonAbstractTaskPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
