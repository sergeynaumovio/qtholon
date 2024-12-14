// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop.h"
#include "holonabstracttask.h"
#include "holonabstracttaskwindow.h"
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
                    if (HolonAbstractTaskWindow *window = qobject_cast<HolonAbstractTaskWindow *>(dockWidget->window()))
                        setTaskWindow(window);
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

void HolonDesktop::addWindow(HolonAbstractWindow *window)
{
    d_ptr->addWindow(window);
}

void HolonDesktop::addWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->addWindowArea(windowArea);
}

void HolonDesktop::addWorkflow(HolonWorkflow *workflow)
{
    d_ptr->addWorkflow(workflow);
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

void HolonDesktop::setTask(HolonAbstractTask *task)
{
    d_ptr->setTask(task);
}

void HolonDesktop::setTaskWindow(HolonAbstractTaskWindow *window)
{
    d_ptr->setTaskWindow(window);
}

void HolonDesktop::setTheme(HolonTheme *theme)
{
    d_ptr->setTheme(theme);
}

void HolonDesktop::setWindowArea(HolonWindowArea *windowArea)
{
    d_ptr->setWindowArea(windowArea);
}

HolonAbstractTask *HolonDesktop::task() const
{
    return d_ptr->task();
}

HolonTheme *HolonDesktop::theme() const
{
    return d_ptr->theme();
}

HolonWorkflow *HolonDesktop::workflow() const
{
    return d_ptr->workflow();
}
