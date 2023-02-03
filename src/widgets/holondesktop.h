// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_H
#define HOLONDESKTOP_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonTask;
class HolonTaskbar;
class HolonWindow;

class Q_HOLON_EXPORT HolonDesktop : public QWidget, public QLoaderSettings
{
    Q_OBJECT

    friend class HolonDesktopPrivate;
    const QScopedPointer<HolonDesktopPrivate> d_ptr;

protected:
    void closeEvent(QCloseEvent *) override;
    void resizeEvent(QResizeEvent *) override;
    QVariant fromString(const QString &string) const override;
    QString fromVariant(const QVariant &variant) const override;

Q_SIGNALS:
    void sizeChanged(QSize);

public:
    HolonDesktop(QLoaderSettings *settings, QWidget *parent);
    ~HolonDesktop();

    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonTask *task);
    void addWindow(HolonWindow *window);
    QString buttonStyleSheet() const;
    int menuBorder() const;
    QString menuStyleSheet() const;
    int menuWidth() const;
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
    QList<HolonWindow *> windowList() const;
};

#endif // HOLONDESKTOP_H
