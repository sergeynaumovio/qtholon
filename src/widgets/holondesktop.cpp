// Copyright (C) 2023 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#include "holondesktop.h"
#include "holondesktop_p.h"
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

void HolonDesktop::addModel(QAbstractItemModel *model, Tasks tasks)
{
    d_ptr->addModel(model, tasks);
}

void HolonDesktop::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

void HolonDesktop::addWindow(HolonWindow *window)
{
    d_ptr->addWindow(window);
}

QString HolonDesktop::buttonStyleSheet() const
{
    return d_ptr->buttonStyleSheet();
}

int HolonDesktop::menuBorder() const
{
    return d_ptr->menuBorder();
}

QString HolonDesktop::menuStyleSheet() const
{
    return d_ptr->menuStyleSheet();
}

int HolonDesktop::menuWidth() const
{
    return d_ptr->menuWidth();
}

QList<QAbstractItemModel *> HolonDesktop::models(Tasks tasks) const
{
    return d_ptr->models(tasks);
}

void HolonDesktop::setCurrentTask(HolonTask *task)
{
    Q_UNUSED(task)
}

int HolonDesktop::titleBarHeight() const
{
    return d_ptr->titleBarHeight();
}

QString HolonDesktop::titleBarStyleSheet() const
{
    return d_ptr->titleBarStyleSheet();
}

QList<HolonWindow *> HolonDesktop::windows() const
{
    return d_ptr->windowList();
}
