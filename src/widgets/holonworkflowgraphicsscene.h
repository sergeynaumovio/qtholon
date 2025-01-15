// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWGRAPHICSSCENE_H
#define HOLONWORKFLOWGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QScopedStorage>

class HolonWorkflow;
class HolonWorkflowGraphicsScenePrivate;

class HolonWorkflowGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

    QScopedStorage<HolonWorkflowGraphicsScenePrivate, 16> d_ptr;

public:
    HolonWorkflowGraphicsScene(HolonWorkflow *parent);
    ~HolonWorkflowGraphicsScene();
};

#endif // HOLONWORKFLOWGRAPHICSSCENE_H
