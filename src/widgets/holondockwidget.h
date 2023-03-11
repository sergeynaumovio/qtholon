// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDOCKWIDGET_H
#define HOLONDOCKWIDGET_H

#include <QDockWidget>
#include <QScopedStorage>

class HolonAbstractWindow;
class HolonDesktop;
class HolonDockWidgetPrivate;
class HolonTitleBar;
class HolonWindowArea;
class HolonWindowAreaPrivate;

class HolonDockWidget : public QDockWidget
{
    Q_OBJECT

    const QScopedStorage<HolonDockWidgetPrivate, 40> d_ptr;

protected:
    void resizeEvent(QResizeEvent *) override;

public:
    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonWindowArea *area);

    HolonDockWidget(HolonDesktop *desktop,
                    QMainWindow *parent,
                    HolonAbstractWindow *window,
                    HolonWindowAreaPrivate *d);

    ~HolonDockWidget();

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);
    HolonTitleBar *titleBar() const;
    HolonAbstractWindow *window() const;
};

#endif // HOLONDOCKWIDGET_H
