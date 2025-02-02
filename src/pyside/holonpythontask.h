// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPYTHONTASK_H
#define HOLONPYTHONTASK_H

class HolonDesktop;
class QWidget;

class HolonPythonTask
{
    friend class HolonDesktopPrivateData;
    HolonDesktop *desktop{};

public:
    QWidget *parametersWidget() const;
};

#endif // HOLONPYTHONTASK_H
