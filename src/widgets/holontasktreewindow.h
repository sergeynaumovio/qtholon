// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONTASKTREEWINDOW_H
#define HOLONTASKTREEWINDOW_H

#include "holonabstractwindow.h"
#include <QWidget>

class HolonDesktop;
class HolonTaskTreeWindowPrivate;
class HolonSidebar;

class HolonTaskTreeWindow : public HolonAbstractWindow
{
    Q_OBJECT

    const QScopedPointer<HolonTaskTreeWindowPrivate> d_ptr;

public:
    HolonTaskTreeWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonTaskTreeWindow(QLoaderSettings *settings, HolonSidebar *parent);
    ~HolonTaskTreeWindow();

    HolonAbstractWindow::Area area() const override;
    HolonAbstractWindow::Attributes attributes() const override;
    QIcon icon() const override;
    bool isCopyable(const QStringList &to) const override;
    QString title() const override;
    QWidget *toolbar() const override;
    QWidget *widget(const QString &group = QString()) const override;
};

#endif // HOLONTASKTREEWINDOW_H
