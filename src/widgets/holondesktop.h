// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_H
#define HOLONDESKTOP_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QWidget>

class HolonAbstractTask;
class HolonAbstractTaskWindow;
class HolonAbstractWindow;
class HolonDesktopPrivate;
class HolonSidebar;
class HolonTaskThread;
class HolonTheme;
class HolonWindowArea;
class HolonWorkflow;

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

public:
    explicit HolonDesktop(QLoaderSettings *settings, QWidget *parent);
    ~HolonDesktop();

    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    void addWorkflow(HolonWorkflow *workflow);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    void closeWindowArea(HolonWindowArea *windowArea);
    void setTask(HolonAbstractTask *task);
    void setTheme(HolonTheme *theme);
    void setWindow(HolonAbstractWindow *window);
    void setWindowArea(HolonWindowArea *windowArea);
    HolonAbstractTask *task() const;
    HolonTaskThread *taskThread() const;
    HolonAbstractTaskWindow *taskWindow() const;
    HolonTheme *theme() const;
    HolonWorkflow *workflow() const;
};

#endif // HOLONDESKTOP_H
