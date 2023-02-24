// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPARAMETERSWINDOW_H
#define HOLONPARAMETERSWINDOW_H

#include "holonabstractwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonParametersWindowPrivate;
class HolonSidebar;

class HolonParametersWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedPointer<HolonParametersWindowPrivate> d_ptr;

public:
    HolonParametersWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonParametersWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonParametersWindow();

    HolonAbstractWindow::Area area() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(HolonAbstractWindow::Area area) const override;
};

#endif // HOLONPARAMETERSWINDOW_H
