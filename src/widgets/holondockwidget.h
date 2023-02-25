// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDOCKWIDGET_H
#define HOLONDOCKWIDGET_H

#include <QDockWidget>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidgetPrivate;
class HolonWindowArea;
class HolonWindowAreaPrivate;

class HolonDockWidget : public QDockWidget
{
    Q_OBJECT

    HolonDockWidgetPrivate &d;
    std::aligned_storage_t<8, sizeof (ptrdiff_t)> d_storage;

    HolonDockWidget(QMainWindow *parent,
                    HolonAbstractWindow *window = nullptr);

public:
    HolonDockWidget(HolonDesktop *desktop,
                    HolonWindowArea *area,
                    QMainWindow *parent);

    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonAbstractWindow *window,
                    HolonWindowAreaPrivate *d);

    HolonAbstractWindow *window() const;
};

#endif // HOLONDOCKWIDGET_H
