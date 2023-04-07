// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTERMINALWINDOW_H
#define HOLONTERMINALWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonTerminalWindowPrivate;
class HolonSidebar;

class HolonTerminalWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonTerminalWindowPrivate, 32> d_ptr;

public:
    HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent);
    ~HolonTerminalWindow();

    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    int role() const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONTERMINALWINDOW_H
