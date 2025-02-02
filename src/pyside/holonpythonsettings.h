// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPYTHONSETTINGS_H
#define HOLONPYTHONSETTINGS_H

#include <QtCore/QVariant>

class HolonDesktop;

class HolonPythonSettings
{
    friend class HolonDesktopPrivateData;
    HolonDesktop *desktop{};

public:
    bool setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
};

#endif // HOLONPYTHONSETTINGS_H
