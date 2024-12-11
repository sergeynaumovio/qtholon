// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONOPENTASKTREEWINDOW_H
#define HOLONOPENTASKTREEWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonOpenTaskTreeWindowPrivate;
class HolonSidebar;
class HolonStackedWindow;

class HolonOpenTaskTreeWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonOpenTaskTreeWindowPrivate, 32> d_ptr;

public:
    HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent);
    HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonStackedWindow *parent);
    ~HolonOpenTaskTreeWindow();

    QWidget *centralWidget() const override;
    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbarWidget() const override;
    QWidget *widget(int role = Holon::NoRole) const override;
};

#endif // HOLONOPENTASKTREEWINDOW_H
