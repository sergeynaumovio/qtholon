// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTERMINALWINDOW_H
#define HOLONTERMINALWINDOW_H

#include "holonabstracttaskwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonTaskStackedWindow;
class HolonTerminalWindowPrivate;

class HolonTerminalWindow : public HolonAbstractTaskWindow
{
    Q_OBJECT

    const QScopedStorage<HolonTerminalWindowPrivate, 48> d_ptr;

public:
    HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent);
    ~HolonTerminalWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    QWidget *settingsWidget() override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONTERMINALWINDOW_H
