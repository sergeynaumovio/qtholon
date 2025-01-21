// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonid.h"
#include <QObject>
#include <QSet>

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

QStringView HolonId::objectName(QStringView section)
{
    return section.sliced(section.lastIndexOf(u'/', -1) + 1);
}

QStringView HolonId::parentSection(QStringView section)
{
    int splitIndex = section.lastIndexOf(u'/', -2);

    return (splitIndex == -1 ? QStringView{} : section.first(splitIndex));
}
