// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONNAMESPACE_H
#define HOLONNAMESPACE_H

#include "qtholonglobal.h"
#include "qtmetamacros.h"

struct QMetaObject;

namespace Holon
{
    Q_NAMESPACE_EXPORT(Q_HOLON_EXPORT)

    enum TreeItem
    {
        Collapsed,
        Expanded
    };
    Q_ENUM_NS(TreeItem)
};

#endif // HOLONNAMESPACE_H
