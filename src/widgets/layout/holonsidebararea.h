/****************************************************************************
**
** Copyright (C) 2021 Sergey Naumov
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

#ifndef HOLONSIDEBARAREA_H
#define HOLONSIDEBARAREA_H

#include "holonstacked.h"
#include <QLoaderSettings>

class HolonMain;
class HolonSidebar;

class HolonSidebarArea : public HolonStacked
{
    Q_OBJECT

    Q_PROPERTY(int stateIndex READ stateIndex WRITE setStateIndex NOTIFY stateIndexChanged)

    int m_stateIndex{-1};

Q_SIGNALS:
    void stateIndexChanged(int i);

public:
    HolonSidebarArea(QLoaderSettings *settings, HolonSplitted *parent);

    bool addSidebar(HolonSidebar *sidebar);
    void addWidget(QWidget *widget) = delete;
    void hide();
    HolonMain *mainWindow() const;
    int stateIndex() const;
    void setStateIndex(int i);
    void show();
};

#endif // HOLONSIDEBARAREA_H
