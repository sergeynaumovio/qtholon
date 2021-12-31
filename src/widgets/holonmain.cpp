/****************************************************************************
**
** Copyright (C) 2021, 2022 Sergey Naumov
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

#include "holonmain.h"
#include "holonmain_p.h"
#include <QRegularExpression>
#include <QStatusBar>

void HolonMain::closeEvent(QCloseEvent*)
{
    hide();
    deleteLater();
}

HolonMain::HolonMain(QLoaderSettings *settings, QWidget *parent)
:   QLoaderSettings(settings),
    d_ptr(new HolonMainPrivate(this))
{
    setParent(parent);

    if (!parent)
        show();

    if (contains("sidebarList"))
    {
        QRegularExpression whiteSpace("\\s+");
        const QStringList sidebarList = value("sidebarList").toString().remove(whiteSpace).split(',');
        for (const QString &s : sidebarList)
        {
            if (s.size() != 1)
            {
                setObjectError("sidebarList item is not a char");
                return;
            }
            d_ptr->sidebarList.append(s.at(0));
        }
    }
    else
    {
        setObjectError("sidebarList property is not set");
        return;
    }

    if (contains("sidebarAreaList"))
    {
        QRegularExpression whiteSpace("\\s+");
        d_ptr->sidebarAreaList = value("sidebarAreaList").toString().remove(whiteSpace).split(',');
    }
    else
    {
        setObjectError("sidebarAreaList property is not set");
        return;
    }

    statusBar()->addWidget(new SidebarSelector(d_ptr.data()), 1);

    setStyleSheet("QStatusBar { background-color : rgb(64, 66, 68) }"
                  "QStatusBar::item { border: 0px }" ) ;
}

HolonMain::~HolonMain()
{ }

QStringList HolonMain::sidebarAreaList() const
{
    return d_ptr->sidebarAreaList;
}

QList<QChar> HolonMain::sidebarList() const
{
    return d_ptr->sidebarList;
}
