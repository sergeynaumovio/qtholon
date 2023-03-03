// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDOCKWIDGET_H
#define HOLONDOCKWIDGET_H

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

    HolonDockWidgetPrivate &d;
    std::aligned_storage_t<24, sizeof (ptrdiff_t)> d_storage;

public:
    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonWindowArea *area);

    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonAbstractWindow *window,
                    HolonWindowAreaPrivate *d);

    HolonTitleBar *titleBar() const;
    HolonAbstractWindow *window() const;
};

#endif // HOLONDOCKWIDGET_H
