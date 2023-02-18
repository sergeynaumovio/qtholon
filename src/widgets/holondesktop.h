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
class HolonTaskModel;
class HolonWindow;
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
    void addWindow(HolonWindow *window);
    void addWindowArea(HolonWindowArea *windowArea);
    QString buttonStyleSheet() const;
    HolonTask *currentTask() const;
    int menuBorder() const;
    QString menuStyleSheet() const;
    int menuWidth() const;
    void setCurrentTask(HolonTask *task);
    HolonTaskModel *taskModel() const;
    int titleBarHeight() const;
    QString titleBarStyleSheet() const;
    QList<HolonWindow *> windows() const;
    HolonWorkflowModel *workflowModel() const;
};

#endif // HOLONDESKTOP_H
