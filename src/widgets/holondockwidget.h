// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDOCKWIDGET_H
#define HOLONDOCKWIDGET_H

#include "holonalignedstorage.h"
#include <QDockWidget>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidgetPrivate;
class HolonTitleBar;
class HolonWindowArea;
class HolonWindowAreaPrivate;

class HolonDockWidget : public QDockWidget
{
    Q_OBJECT

    const HolonAlignedStorage<HolonDockWidgetPrivate, 24> d_ptr;

public:
    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonWindowArea *area);

    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonAbstractWindow *window,
                    HolonWindowAreaPrivate *d);

    ~HolonDockWidget();

    HolonTitleBar *titleBar() const;
    HolonAbstractWindow *window() const;
};

#endif // HOLONDOCKWIDGET_H
