// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwidget.h"
#include "holonabstracttask.h"

HolonAbstractWidget::HolonAbstractWidget(QLoaderSettings *settings, HolonAbstractTask *parent)
:   QObject(parent),
    QLoaderSettings(settings)
{ }

HolonAbstractWidget::~HolonAbstractWidget()
{ }

QString HolonAbstractWidget::group() const
{
    return value("group").toString();
}
