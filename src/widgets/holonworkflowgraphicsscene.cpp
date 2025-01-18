// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowgraphicsscene.h"
#include "holonworkflow.h"
#include <QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class HolonGraphicsTaskItem : public QGraphicsPolygonItem
{
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override
    {
        event->accept();
    }

public:
    enum { Type = UserType};

    HolonGraphicsTaskItem()
    {
        setFlag(QGraphicsItem::ItemIsSelectable, true);
        setPolygon(QRectF(0, 0, 100, 50));
    }
};

class HolonWorkflowGraphicsScenePrivate
{
    HolonWorkflowGraphicsScene *const q_ptr;
    HolonWorkflow *workflow;

public:
    HolonWorkflowGraphicsScenePrivate(HolonWorkflowGraphicsScene *q, HolonWorkflow *w)
    :   q_ptr(q),
        workflow(w)
    {
        Q_UNUSED(workflow)

        addItem();
    }

    void addItem()
    {
         HolonGraphicsTaskItem *item = new HolonGraphicsTaskItem;
         item->setBrush(QColor(Qt::white));
         item->setPos(20, 20);
         q_ptr->addItem(item);
    }
};

HolonWorkflowGraphicsScene::HolonWorkflowGraphicsScene(HolonWorkflow *workflow)
:   QGraphicsScene(workflow),
    d_ptr(this, workflow)
{
    setSceneRect(QRectF(0, 0, 200, 100));
}

HolonWorkflowGraphicsScene::~HolonWorkflowGraphicsScene()
{ }
