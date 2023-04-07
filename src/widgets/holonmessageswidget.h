// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMESSAGESWIDGET_H
#define HOLONMESSAGESWIDGET_H

#include "holonabstractwidget.h"
#include <QScopedStorage>

class HolonAbstractTask;
class HolonMessagesWidgetPrivate;

class HolonMessagesWidget : public HolonAbstractWidget
{
    Q_OBJECT

    const QScopedStorage<HolonMessagesWidgetPrivate, 8> d_ptr;

public:
    HolonMessagesWidget(QLoaderSettings *settings, HolonAbstractTask *parent);
    ~HolonMessagesWidget();

    int role() const override;
    QWidget *widget() const override;
};

#endif // HOLONMESSAGESWIDGET_H
