// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop.h"
#include "holonabstracttask.h"
#include "holonabstractwindow.h"
#include "holondesktop_p.h"
#include "holondockwidget.h"
#include <QApplication>
#include <QLoaderTree>

void HolonDesktop::closeEvent(QCloseEvent *)
{
    hide();
    deleteLater();
}

void HolonDesktop::resizeEvent(QResizeEvent *e)
{
    d_ptr->resizeEvent(e);
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
    QLoaderSettings(settings),
    d_ptr(new HolonDesktopPrivate(this))
{
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

    if (!parent)
        show();

    d_ptr->setLayout();
}

HolonDesktop::~HolonDesktop()
{ }

void HolonDesktop::addModel(HolonTaskModel *model)
{
    d_ptr->addModel(model);
}

void HolonDesktop::addModel(HolonWorkflowModel *model)
{
    d_ptr->addModel(model);
}

void HolonDesktop::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

void HolonDesktop::addTask(HolonAbstractTask *task)
{
    d_ptr->addTask(task);
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

QString HolonDesktop::buttonStyleSheet() const
{
    return d_ptr->buttonStyleSheet();
}

void HolonDesktop::closeTask(HolonAbstractTask *task)
{
    d_ptr->closeTask(task);
}

void HolonDesktop::closeWindow(HolonAbstractWindow *window)
{
    d_ptr->closeWindow(window);
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

void HolonDesktop::hideWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->hideWindowArea(windowArea);
}

int HolonDesktop::menuBorderWidth() const
{
    return d_ptr->menuBorderWidth();
}

QString HolonDesktop::menuStyleSheet() const
{
    return d_ptr->menuStyleSheet();
}

int HolonDesktop::menuWidth() const
{
    return d_ptr->menuWidth();
}

void HolonDesktop::setCurrentTask(HolonAbstractTask *task)
{
    d_ptr->setCurrentTask(task);
}

void HolonDesktop::setCurrentTaskModel(HolonTaskModel *model)
{
    d_ptr->setCurrentTaskModel(model);
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

void HolonDesktop::showWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->showWindowArea(windowArea);
}

int HolonDesktop::titleBarHeight() const
{
    return d_ptr->titleBarHeight();
}

QString HolonDesktop::titleBarStyleSheet() const
{
    return d_ptr->titleBarStyleSheet();
}

QList<HolonAbstractWindow *> HolonDesktop::windows() const
{
    return d_ptr->windows();
}

