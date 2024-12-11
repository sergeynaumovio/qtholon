// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTERMINALWINDOW_H
#define HOLONTERMINALWINDOW_H

#include "holonabstracttaskwindow.h"

class HolonDesktop;
class HolonTerminalWindowPrivate;
class HolonStackedWindow;

class HolonTerminalWindow : public HolonAbstractTaskWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonTerminalWindow)

public:
    HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonStackedWindow *parent);
    ~HolonTerminalWindow();

    QWidget *centralWidget() const override;
    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    int role() const override;
    QWidget *settingsWidget() const override;
    QString title() const override;
    QWidget *toolbarWidget() const override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONTERMINALWINDOW_H
