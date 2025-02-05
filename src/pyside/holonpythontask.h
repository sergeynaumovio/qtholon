// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPYTHONTASK_H
#define HOLONPYTHONTASK_H

#include <QtCore/QVariant>

class HolonDesktop;
class QWidget;

class HolonPythonTask
{
    friend class HolonDesktopPrivateData;
    HolonDesktop *desktop{};

public:
    QWidget *parametersWidget() const;
    bool setValue(const QString &key, const QVariant &value);
    QVariant value(const QString &key, const QVariant &defaultValue = QVariant()) const;
};

#endif // HOLONPYTHONTASK_H
