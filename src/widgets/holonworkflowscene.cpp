// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonworkflowscene.h"
#include "holonworkflowitem.h"

HolonWorkflowScene::HolonWorkflowScene(QObject *parent)
:   QGraphicsScene(parent)
{
    HolonWorkflowItem *item = new HolonWorkflowItem(HolonWorkflowItem::Task);
    item->setBrush(QColor(Qt::white));
    item->setPos(20, 20);
    addItem(item);

    setSceneRect(QRectF(0, 0, 200, 100));
}

HolonWorkflowScene::~HolonWorkflowScene()
{ }
