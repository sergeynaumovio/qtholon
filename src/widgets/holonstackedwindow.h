// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWINDOW_H
#define HOLONSTACKEDWINDOW_H

#include "holonabstractwindow.h"

class HolonStackedWindowPrivate;

class Q_HOLON_EXPORT HolonStackedWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonStackedWindow)

    friend class HolonWindowAreaPrivate;

public:
    HolonStackedWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonStackedWindow(QLoaderSettings *settings, HolonWindowArea *parent);

    ~HolonStackedWindow();

    QWidget *centralWidget() const override;
    void addWindow(HolonAbstractWindow *window);
    HolonDesktop *desktop() const;
    bool isCopyable(const QStringList &to) const override;
    void setWindow(HolonAbstractWindow *window);
    QWidget *toolbarWidget() const override;
};

#endif // HOLONSTACKEDWINDOW_H
