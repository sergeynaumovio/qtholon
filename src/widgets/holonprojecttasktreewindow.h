// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPROJECTTASKTREEWINDOW_H
#define HOLONPROJECTTASKTREEWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonProjectTaskTreeWindowPrivate;
class HolonSidebar;
class HolonStackedWindow;

class HolonProjectTaskTreeWindow : public HolonAbstractWindow
{
    Q_OBJECT

    friend class HolonDesktopPrivateData;
    const QScopedStorage<HolonProjectTaskTreeWindowPrivate, 48> d_ptr;

public:
    HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent);
    Q_INVOKABLE HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonStackedWindow *parent);
    ~HolonProjectTaskTreeWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONPROJECTTASKTREEWINDOW_H
