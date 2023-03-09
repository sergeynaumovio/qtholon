// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_H
#define HOLONPARAMETERSWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonParametersWindowPrivate;
class HolonSidebar;

class HolonParametersWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonParametersWindowPrivate, 40> d_ptr;

public:
    HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonParametersWindow();

    HolonAbstractWindow::Areas areas() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(const QString &group = QString()) const override;
};

#endif // HOLONPARAMETERSWINDOW_H
