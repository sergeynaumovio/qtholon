// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonabstractwidget.h"
#include "holonabstracttask.h"
#include "holonnamespace.h"

using namespace Qt::Literals::StringLiterals;

HolonAbstractWidget::HolonAbstractWidget(QLoaderSettings *settings, HolonAbstractTask *parent)
:   QObject(parent),
    QLoaderSettings(this, settings)
{ }

HolonAbstractWidget::~HolonAbstractWidget()
{ }

int HolonAbstractWidget::role() const
{
    return Holon::NoRole;
}
