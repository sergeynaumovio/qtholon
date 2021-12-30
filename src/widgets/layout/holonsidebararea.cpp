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

#include "holonsidebararea.h"
#include "holonstacked.h"
#include "holonsplitted.h"
#include "holonmain.h"
#include "holonmain_p.h"

HolonSidebarArea::HolonSidebarArea(QLoaderSettings *settings, HolonSplitted *parent)
:   HolonStacked(settings, parent)
{
    if (mainWindow())
    {
        if (!mainWindow()->d_ptr->mapSidebarArea(section().last()))
        {
            setObjectError("SidebarArea name is not in list or already used");
            return;
        }

        if (contains("stateIndex"))
        {
            setStateIndex(value("stateIndex").toInt());
        }

        connect(mainWindow()->d_ptr->sidebarActivator, &SidebarActivator::sidebarButtonClicked, this, [this](QChar sidebar, QString area)
        {
            if (objectName() == area)
            {
                for (int i = 0; i < count(); ++i)
                {
                    if (widget(i)->objectName() == sidebar)
                    {
                        if (currentIndex() == i)
                        {
                            if (isVisible())
                                hide();
                            else
                                show();

                            break;
                        }

                        setCurrentIndex(i);
                        show();
                    }
                }
            }
        });
    }
}

HolonMain *HolonSidebarArea::mainWindow() const
{
    return static_cast<HolonSplitted*>(parent())->mainWindow();
}

int HolonSidebarArea::stateIndex() const
{
    return m_stateIndex;
}

void HolonSidebarArea::setStateIndex(int i)
{
    m_stateIndex = i;
    emit stateIndexChanged(i);
}
