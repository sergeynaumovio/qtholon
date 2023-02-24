// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTERMINALWINDOW_H
#define HOLONTERMINALWINDOW_H

#include "holonabstractwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonTerminalWindowPrivate;
class HolonSidebar;

class HolonTerminalWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedPointer<HolonTerminalWindowPrivate> d_ptr;

public:
    HolonTerminalWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTerminalWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTerminalWindow();

    Area area() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(HolonAbstractWindow::Area area) const override;
};

#endif // HOLONTERMINALWINDOW_H
