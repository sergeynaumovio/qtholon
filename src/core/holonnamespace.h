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

    enum TreeItem
    {
        Collapsed,
        Expanded
    };
    Q_ENUM_NS(TreeItem)

    enum WidgetRole
    {
        NoRole = 0,
        ParametersRole = 1,
        SettingsRole = 2,
        MessagesRole = 3
    };
    Q_ENUM_NS(WidgetRole)
};

#endif // HOLONNAMESPACE_H
