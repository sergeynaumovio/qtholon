// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTERMINALWINDOW_H
#define HOLONTERMINALWINDOW_H

#include "holonabstracttaskwindow.h"

class HolonDesktop;
class HolonTaskStackedWindow;
class HolonTerminalWindowPrivate;

class HolonTerminalWindow : public HolonAbstractTaskWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTerminalWindow)

public:
    HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent);
    ~HolonTerminalWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QWidget *settingsWidget() override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONTERMINALWINDOW_H
