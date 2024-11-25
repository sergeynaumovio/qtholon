// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONDESKTOP_H
#define HOLONDESKTOP_H

#include "qtholonglobal.h"
#include <QLoaderSettings>
#include <QWidget>

class HolonAbstractTask;
class HolonAbstractWindow;
class HolonDesktopPrivate;
class HolonSidebar;
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
    void addTheme(HolonTheme *theme);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    void addWorkflow(HolonWorkflow *workflow);
    void closeTask(HolonAbstractTask *task);
    void closeWindow(HolonAbstractWindow *window);
    void closeWindowArea(HolonWindowArea *windowArea);
    HolonAbstractTask *currentTask() const;
    HolonTheme *currentTheme() const;
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentTheme(HolonTheme *theme);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    QList<HolonAbstractWindow *> windows() const;
    HolonWorkflow *workflow() const;
};

#endif // HOLONDESKTOP_H
