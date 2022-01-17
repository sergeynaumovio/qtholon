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

#ifndef HOLONSIDEBARAREA_H
#define HOLONSIDEBARAREA_H

#include "holonstacked.h"
#include <QLoaderSettings>

class HolonDesktop;
class HolonSidebar;

class HolonSidebarArea : public HolonStacked
{
    Q_OBJECT

    int m_stateIndex{-1};
    void setStateIndex(int i);

public:
    HolonSidebarArea(QLoaderSettings *settings, HolonSplitted *parent);

    bool addSidebar(HolonSidebar *sidebar);
    void addWidget(QWidget *widget) = delete;
    void hide();
    HolonDesktop *desktop() const;
    void setCurrentIndex(int i);
    void setCurrentWidget(HolonSidebar *sidebar);
    void show();
    int stateIndex() const { return m_stateIndex; }
};

#endif // HOLONSIDEBARAREA_H
