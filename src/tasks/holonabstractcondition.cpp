// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractcondition.h"
#include "holondesktop.h"
#include "holontheme.h"
#include "holonthemeicons.h"

using namespace Qt::Literals::StringLiterals;

HolonAbstractCondition::HolonAbstractCondition(QLoaderSettings *settings, HolonTaskFolder *folder)
:   HolonAbstractTask(settings, folder)
{ }

HolonAbstractCondition::HolonAbstractCondition(QLoaderSettings *settings, HolonWorkflow *workflow)
:   HolonAbstractTask(settings, workflow)
{ }

HolonAbstractCondition::~HolonAbstractCondition()
{ }

QIcon HolonAbstractCondition::icon() const
{
    return desktop()->theme()->icons()->taskIcon();
}
