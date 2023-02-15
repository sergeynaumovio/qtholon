// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_H
#define HOLONDESKTOP_H

#include "holonnamespace.h"
#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QWidget>

class HolonDesktopPrivate;
class HolonSidebar;
class HolonTask;
class HolonTaskbar;
class HolonWindow;
class HolonWindowArea;
class QAbstractItemModel;

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
    void addModel(QAbstractItemModel *model, Holon::Tasks tasks);
    void addWindow(HolonWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    QString buttonStyleSheet() const;
    HolonTask *currentTask() const;
    int menuBorder() const;
    QString menuStyleSheet() const;
    int menuWidth() const;
    QList<QAbstractItemModel *> models(Holon::Tasks tasks) const;
    void setCurrentTask(HolonTask *task);
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
    QList<HolonWindow *> windows() const;
};

#endif // HOLONDESKTOP_H
