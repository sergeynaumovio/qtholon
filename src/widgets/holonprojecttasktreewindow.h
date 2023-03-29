// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPROJECTTASKTREEWINDOW_H
#define HOLONPROJECTTASKTREEWINDOW_H

#include "holonabstractwindow.h"
#include <QScopedStorage>

class HolonDesktop;
class HolonProjectTaskTreeWindowPrivate;
class HolonSidebar;

class HolonProjectTaskTreeWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedStorage<HolonProjectTaskTreeWindowPrivate, 40> d_ptr;

public:
    HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonProjectTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonProjectTaskTreeWindow();

    Holon::WindowFlags flags() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(const QString &group = QString()) const override;
};

#endif // HOLONPROJECTTASKTREEWINDOW_H
