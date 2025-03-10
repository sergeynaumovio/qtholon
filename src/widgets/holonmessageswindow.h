// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
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

    const QScopedStorage<HolonMessagesWindowPrivate, 32> d_ptr;

public:
    HolonMessagesWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonMessagesWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonMessagesWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONMESSAGESWINDOW_H
