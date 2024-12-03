// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonid.h"

uint HolonId::createChildId(QObject *parent)
{
    uint index;
    QSet<uint> children;
    for (QObject *o : parent->children())
        if (bool ok = (index = o->objectName().toUInt(&ok), ok))
            children.insert(index);

    int id{-1};
    while (children.contains(++id));

    return id;
}
