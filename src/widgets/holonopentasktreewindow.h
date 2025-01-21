// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
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
    Q_INVOKABLE HolonOpenTaskTreeWindow(QLoaderSettings *settings, HolonStackedWindow *parent);
    ~HolonOpenTaskTreeWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONOPENTASKTREEWINDOW_H
