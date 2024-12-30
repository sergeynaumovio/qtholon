// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holontoolbar.h"
#include <QApplication>
#include <QBoxLayout>
#include <QStyleOption>
#include <QToolButton>

HolonToolBar::HolonToolBar()
{
    setFixedHeight(QApplication::style()->pixelMetric(QStyle::PM_TitleBarHeight));
    layout()->setSpacing(0);
    layout()->setContentsMargins({});
}

HolonToolBar::~HolonToolBar()
{ }

QToolButton *HolonToolBar::toolButton(QIcon icon)
{
    QToolButton *button = new QToolButton(this);
    button->setFixedHeight(height());
    button->setFixedWidth(button->height() * 1.2);
    button->setIcon(icon);

    return button;
}
