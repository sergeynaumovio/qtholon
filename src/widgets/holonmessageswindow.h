// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMESSAGESWINDOW_H
#define HOLONMESSAGESWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonMessagesWindowPrivate;
class HolonSidebar;

class HolonMessagesWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonMessagesWindowPrivate, 40> d_ptr;

public:
    HolonMessagesWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonMessagesWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonMessagesWindow();

    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    int role() const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONMESSAGESWINDOW_H
