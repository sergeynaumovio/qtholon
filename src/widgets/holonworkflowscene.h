// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONWORKFLOWSCENE_H
#define HOLONWORKFLOWSCENE_H

#include <QGraphicsScene>

class HolonWorkflowScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit HolonWorkflowScene(QObject *parent);
    ~HolonWorkflowScene();
};

#endif // HOLONWORKFLOWSCENE_H
