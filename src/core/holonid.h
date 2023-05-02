// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONID_H
#define HOLONID_H

#include <QObject>
#include <QSet>

namespace HolonId
{

uint createChildId(QObject *parent);

} // namespace HolonId

#endif // HOLONID_H
