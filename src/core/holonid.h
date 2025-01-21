// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONID_H
#define HOLONID_H

#include "qtholonglobal.h"
#include <QStringView>

class QObject;

namespace HolonId
{

Q_HOLON_EXPORT uint createChildId(QObject *parent);
Q_HOLON_EXPORT QStringView objectName(QStringView section);
Q_HOLON_EXPORT QStringView parentSection(QStringView section);

} // namespace HolonId

#endif // HOLONID_H
