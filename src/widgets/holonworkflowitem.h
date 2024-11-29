// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWITEM_H
#define HOLONWORKFLOWITEM_H

#include <QGraphicsPolygonItem>

class HolonWorkflowItem : public QGraphicsPolygonItem
{
public:
    enum { Type = UserType};
    enum ItemType { Task, Condition, Adress };

    HolonWorkflowItem(ItemType type);
    ~HolonWorkflowItem();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
};

#endif // HOLONWORKFLOWITEM_H
