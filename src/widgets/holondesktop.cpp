/****************************************************************************
**
** Copyright (C) 2022-2023 Sergey Naumov
**
** Permission to use, copy, modify, and/or distribute this
** software for any purpose with or without fee is hereby granted.
**
** THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL
** WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED
** WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL
** THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR
** CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
** LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
** NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
** CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
**
****************************************************************************/

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

void HolonDesktop::addSidebar(HolonSidebar *sidebar)
{
    d_ptr->addSidebar(sidebar);
}

void HolonDesktop::addTask(HolonTask* /*task*/)
{

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

int HolonDesktop::titleBarHeight() const
{
    return d_ptr->titleBarHeight();
}

QString HolonDesktop::titleBarStyleSheet() const
{
    return d_ptr->titleBarStyleSheet();
}

QList<HolonWindow *> HolonDesktop::windowList() const
{
    return d_ptr->windowList();
}
