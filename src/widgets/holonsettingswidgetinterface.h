// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSETTINGSWIDGETINTERFACE_H
#define HOLONSETTINGSWIDGETINTERFACE_H

#include <QObject>

class HolonSettingsWidgetInterface
{
public:
    virtual QWidget *settingsWidget() = 0;
};
Q_DECLARE_INTERFACE(HolonSettingsWidgetInterface, "HolonSettingsWidgetInterface")

#endif // HOLONSETTINGSWIDGETINTERFACE_H
