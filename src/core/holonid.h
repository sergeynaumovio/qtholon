// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONID_H
#define HOLONID_H

#include <QObject>
#include <QSet>

namespace HolonId
{

template<typename T>
uint createChildId(QObject *parent)
{
    QSet<uint> children;
    for (QObject *o : parent->children())
    {
        if (qobject_cast<T>(o))
            children.insert(o->objectName().toUInt());
    }

    int id{-1};
    while (children.contains(++id));

    return id;
}

} // namespace HolonId

#endif // HOLONID_H
