// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMARKUPCOMMANDERWINDOW_H
#define HOLONMARKUPCOMMANDERWINDOW_H

#include "holonwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonMarkupCommanderWindowPrivate;
class HolonSidebar;

class HolonMarkupCommanderWindow : public HolonWindow
{
    Q_OBJECT

    const QScopedPointer<HolonMarkupCommanderWindowPrivate> d_ptr;

public:
    HolonMarkupCommanderWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonMarkupCommanderWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonMarkupCommanderWindow();

    HolonWindow::Areas areas() const override;
    HolonWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget() const override;
};

#endif // HOLONMARKUPCOMMANDERWINDOW_H
