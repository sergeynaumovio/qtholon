// Copyright (C) 2026 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONSTACKEDWINDOW_H
#define HOLONSTACKEDWINDOW_H

#include "holonabstractwindow.h"

class HolonStackedWindowPrivate;

class Q_HOLON_EXPORT HolonStackedWindow : public HolonAbstractWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonStackedWindow)

protected:
    HolonStackedWindow(HolonStackedWindowPrivate &d, QLoaderSettings *settings, QObject *parent);

public:
    HolonStackedWindow(QLoaderSettings *settings, HolonWindowArea *parent);

    ~HolonStackedWindow();

    QWidget *centralWidget() override;
    void addWindow(HolonAbstractWindow *window);
    HolonAbstractWindow *currentWindow() const;
    HolonDesktop *desktop() const;
    bool isCopyable(QStringView to) const override;
    void setCurrentWindow(HolonAbstractWindow *window);
    QWidget *toolbarWidget() override;
};

#endif // HOLONSTACKEDWINDOW_H
