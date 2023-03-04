// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holonmessageswidget.h"
#include "holonabstracttask.h"
#include <QPlainTextEdit>

class HolonMessagesWidgetPrivate
{
public:
    QPlainTextEdit *plainText{};

    QWidget *widget()
    {
        if (plainText)
            return plainText;

        plainText = new QPlainTextEdit;

        return plainText;
    }
};

HolonMessagesWidget::HolonMessagesWidget(QLoaderSettings *settings, HolonAbstractTask *parent)
:   HolonAbstractWidget(settings, parent)
{
    parent->addWidget(this);
}

HolonMessagesWidget::~HolonMessagesWidget()
{ }

QWidget *HolonMessagesWidget::widget() const
{
    return d_ptr->plainText;
}

