// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontaskmodel_p.h"
#include "holontaskmodel.h"
#include <QModelIndex>

using namespace Qt::Literals::StringLiterals;

HolonTaskModelPrivate::HolonTaskModelPrivate(HolonTaskModel *q, HolonDesktop *desk)
:   q_ptr(q),
    desktop(desk)
{ }

QObject *HolonTaskModelPrivate::object(const QModelIndex &index) const
{
    if (index.isValid())
        if (QObject *obj = static_cast<QObject *>(index.internalPointer()))
            return obj;

    return q_ptr;
}

void HolonTaskModelPrivate::setCurrent(bool current)
{
    q_ptr->setValue(u"current"_s, current);
}
