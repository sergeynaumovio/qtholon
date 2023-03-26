// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
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
class HolonTaskModel;
class HolonTheme;
class HolonWindowArea;
class HolonWorkflowModel;

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

    void addModel(HolonTaskModel *model);
    void addModel(HolonWorkflowModel *model);
    void addSidebar(HolonSidebar *sidebar);
    void addTask(HolonAbstractTask *task);
    void addTheme(HolonTheme *theme);
    void addWindow(HolonAbstractWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    QString buttonStyleSheet() const;
    void closeWindow(HolonAbstractWindow *window);
    HolonAbstractTask *currentTask() const;
    HolonTheme *currentTheme() const;
    void hideWindowArea(HolonWindowArea *windowArea);
    int menuBorderWidth() const;
    QString menuStyleSheet() const;
    int menuWidth() const;
    void setCurrentTask(HolonAbstractTask *task);
    void setCurrentTheme(HolonTheme *theme);
    void setCurrentWindow(HolonAbstractWindow *window);
    void setCurrentWindowArea(HolonWindowArea *windowArea);
    void showWindowArea(HolonWindowArea *windowArea);
    HolonTaskModel *taskModel() const;
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
    QList<HolonAbstractWindow *> windows() const;
    HolonWorkflowModel *workflowModel() const;
};

#endif // HOLONDESKTOP_H
