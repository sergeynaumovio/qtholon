// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowitem.h"

HolonWorkflowItem::HolonWorkflowItem(ItemType type)
:   QGraphicsPolygonItem()
{
    Q_UNUSED(type)

    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setPolygon(QRectF(0, 0, 100, 50));
}

HolonWorkflowItem::~HolonWorkflowItem()
{ }

void HolonWorkflowItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    return QGraphicsPolygonItem::mousePressEvent(event);
}
