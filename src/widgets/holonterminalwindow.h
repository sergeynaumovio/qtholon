// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTERMINALWINDOW_H
#define HOLONTERMINALWINDOW_H

#include "holonwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonTerminalWindowPrivate;
class HolonSidebar;

class HolonTerminalWindow : public HolonWindow
{
    Q_OBJECT

    const QScopedPointer<HolonTerminalWindowPrivate> d_ptr;

public:
    HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTerminalWindow();

    HolonWindow::Areas areas() const override;
    HolonWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;
};

#endif // HOLONTERMINALWINDOW_H
