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

    enum WindowType
    {
        SidebarWindow = 0x0001,
        TaskWindow = 0x0002,
        WindowSplitButtonHint = 0x0004,
        WindowMinMaxButtonsHint = 0x0008,
        WindowCloseButtonHint = 0x0010,
        WindowAllButtonsHint = WindowSplitButtonHint |
                               WindowMinMaxButtonsHint |
                               WindowCloseButtonHint
    };
    Q_DECLARE_FLAGS(WindowFlags, WindowType)
    Q_DECLARE_OPERATORS_FOR_FLAGS(WindowFlags)
    Q_FLAG_NS(WindowFlags)
};

#endif // HOLONNAMESPACE_H
