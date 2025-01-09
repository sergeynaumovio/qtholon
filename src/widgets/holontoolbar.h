// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTOOLBAR_H
#define HOLONTOOLBAR_H

#include "qtholonglobal.h"
#include <QToolBar>

class QToolButton;

class Q_HOLON_EXPORT HolonToolBar : public QToolBar
{
    Q_OBJECT

public:
    HolonToolBar();
    ~HolonToolBar();

    QToolButton *toolButton(QIcon icon, const QString &toolTip);
};

#endif // HOLONTOOLBAR_H
