// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondesktop_p.h"
#include "holondockwidget.h"
#include <QApplication>
#include <QLoaderTree>

using namespace Qt::Literals::StringLiterals;

void HolonDesktop::closeEvent(QCloseEvent *)
{
    hide();
    deleteLater();
}

void HolonDesktop::resizeEvent(QResizeEvent *)
{
    setValue(u"geometry"_s, saveGeometry());
}

QVariant HolonDesktop::fromString(const QString &string) const
{
    return QLoaderSettings::fromString(string);
}

QString HolonDesktop::fromVariant(const QVariant &variant) const
{
    return QLoaderSettings::fromVariant(variant);
}

HolonDesktop::HolonDesktop(QLoaderSettings *settings, QWidget *parent)
:   QWidget(parent),
    QLoaderSettings(this, settings),
    d_ptr(new HolonDesktopPrivate(this))
{
    restoreGeometry(value(u"geometry"_s).toByteArray());

    if (QApplication *app = qobject_cast<QApplication *>(QCoreApplication::instance()))
    {
        connect(app, &QApplication::focusObjectChanged, this, [this](QObject *object)
        {
            if (object)
                if (HolonDockWidget *dockWidget = qobject_cast<HolonDockWidget *>(object->parent()))
                    if (HolonAbstractWindow *window = dockWidget->window())
                        if (qobject_cast<HolonAbstractTask *>(window->parent()))
                            setCurrentWindow(window);
        });
    }

    connect(tree(), &QLoaderTree::errorChanged, this, [](QObject *sender, QString message)
    {
        qDebug().noquote().nospace() << "QLoaderTree::errorChanged("
                                     << sender->metaObject()->className() << ", \"" << message << "\")";
    });

    connect(tree(), &QLoaderTree::infoChanged, this, [](QObject *sender, QString message)
    {
        qDebug().noquote().nospace() << "QLoaderTree::infoChanged("
                                     << sender->metaObject()->className() << ", \"" << message << "\")";
    });

    connect(tree(), &QLoaderTree::warningChanged, this, [](QObject *sender, QString message)
    {
        qDebug().noquote().nospace() << "QLoaderTree::warningChanged("
                                     << sender->metaObject()->className() << ", \"" << message << "\")";
    });

    connect(tree(), &QLoaderTree::loaded, this, [=, this]()
    {
        if (!parent)
            show();
    });

    d_ptr->setLayout();
}

HolonDesktop::~HolonDesktop()
{ }

void HolonDesktop::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

void HolonDesktop::addTask(HolonAbstractTask *task)
{
    d_ptr->addTask(task);
}

void HolonDesktop::addTaskModel(HolonTaskModel *taskModel)
{
    d_ptr->addTaskModel(taskModel);
}

void HolonDesktop::addTheme(HolonTheme *theme)
{
    d_ptr->addTheme(theme);
}

void HolonDesktop::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
}

void HolonDesktop::addWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->addWindowArea(windowArea);
}

void HolonDesktop::addWorkflowModel(HolonWorkflowModel *workflowModel)
{
    d_ptr->addWorkflowModel(workflowModel);
}

void HolonDesktop::closeTask(HolonAbstractTask *task)
{
    d_ptr->closeTask(task);
}

void HolonDesktop::closeWindow(HolonAbstractWindow *window)
{
    d_ptr->closeWindow(window);
}

void HolonDesktop::closeWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->closeWindowArea(windowArea);
}

HolonAbstractTask *HolonDesktop::currentTask() const
{
    return d_ptr->currentTask();
}

HolonTaskModel *HolonDesktop::currentTaskModel() const
{
    return d_ptr->currentTaskModel();
}

HolonTheme *HolonDesktop::currentTheme() const
{
    return d_ptr->currentTheme();
}

HolonWorkflowModel *HolonDesktop::currentWorkflowModel() const
{
    return d_ptr->currentWorkflowModel();
}

void HolonDesktop::setCurrentTask(HolonAbstractTask *task)
{
    d_ptr->setCurrentTask(task);
}

void HolonDesktop::setCurrentTaskModel(HolonTaskModel *taskModel)
{
    d_ptr->setCurrentTaskModel(taskModel);
}

void HolonDesktop::setCurrentTheme(HolonTheme *theme)
{
    d_ptr->setCurrentTheme(theme);
}

void HolonDesktop::setCurrentWindow(HolonAbstractWindow *window)
{
    d_ptr->setCurrentWindow(window);
}

void HolonDesktop::setCurrentWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->setCurrentWindowArea(windowArea);
}

void HolonDesktop::setCurrentWorkflowModel(HolonWorkflowModel *workflowModel)
{
    d_ptr->setCurrentWorkflowModel(workflowModel);
}

QList<HolonAbstractWindow *> HolonDesktop::windows() const
{
    return d_ptr->windows();
}
