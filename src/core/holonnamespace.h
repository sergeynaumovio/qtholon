// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONNAMESPACE_H
#define HOLONNAMESPACE_H

#include "qtholonglobal.h"
#include "qtmetamacros.h"

struct QMetaObject;

namespace Holon
{
    Q_NAMESPACE_EXPORT(Q_HOLON_EXPORT)

    enum Tasks
    {
        NewTasks,
        OpenTasks
    };
    Q_ENUM_NS(Tasks)
};

#endif // HOLONNAMESPACE_H
