// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPYTHONBINDINGS_H
#define HOLONPYTHONBINDINGS_H

#include <QtCore/QVariant>

class HolonPythonBindings
{
    friend class HolonDesktopPrivateData;

    static bool init();
    static bool bind(int objectIndex, const char *objectName, void *object);
};

#endif // HOLONPYTHONBINDINGS_H
