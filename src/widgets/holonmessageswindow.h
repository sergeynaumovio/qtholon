// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMESSAGESWINDOW_H
#define HOLONMESSAGESWINDOW_H

#include "holonabstractwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonMessagesWindowPrivate;
class HolonSidebar;

class HolonMessagesWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedPointer<HolonMessagesWindowPrivate> d_ptr;

public:
    HolonMessagesWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonMessagesWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonMessagesWindow();

    HolonAbstractWindow::Areas areas() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(const QString &group = QString()) const override;
};

#endif // HOLONMESSAGESWINDOW_H
