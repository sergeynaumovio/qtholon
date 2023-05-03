// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowitem.h"

HolonWorkflowItem::~HolonWorkflowItem()
{
    qDeleteAll(children);
}
