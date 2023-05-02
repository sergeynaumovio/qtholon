// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonid.h"

uint HolonId::createChildId(QObject *parent)
{
    QSet<uint> children;
    for (QObject *o : parent->children())
        children.insert(o->objectName().toUInt());

    int id{-1};
    while (children.contains(++id));

    return id;
}
