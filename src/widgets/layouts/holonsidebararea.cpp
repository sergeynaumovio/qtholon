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

#include "holonsidebararea.h"
#include "holonstacked.h"
#include "holonsplitted.h"
#include "holondesktop.h"
#include "holondesktop_p.h"
#include "holonsidebar.h"

#include <QLoaderTree>
#include <algorithm>

HolonSidebarArea::HolonSidebarArea(QLoaderSettings *settings, HolonSplitted *parent)
:   HolonStacked(settings, parent)
{
    if (!desktop())
    {
        emitError("HolonDesktop not found");
        return;
    }

    if (!desktop()->d_ptr->mapSidebarArea(section().last(), this))
    {
        emitError("sidebar area name is not in list or already used");
        return;
    }

    if (contains("stateIndex"))
        setStateIndex(value("stateIndex").toInt());

    if (!desktop()->d_ptr->sidebarActivator)
        return;

    connect(desktop()->d_ptr->sidebarActivator, &SidebarActivator::sidebarButtonClicked, this, [this](QChar sidebar, QString area)
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

                    break;
                }
            }
        }
    });
}

void HolonSidebarArea::setStateIndex(int i)
{
    m_stateIndex = i;
    setValue("stateIndex", i);
}

bool HolonSidebarArea::addSidebar(HolonSidebar *sidebar)
{
    QStringList src(sidebar->section());

    QStringList dst = section();
    dst.append(src.last());

    if (std::equal(src.begin(), src.end(), dst.begin()))
    {
        QStackedWidget::addWidget(sidebar);
        return true;
    }

    if (tree()->move(src, dst))
    {
        SidebarRelatedObjects &sidebarObjects = desktop()->d_ptr->sidebarRelatedObjects[sidebar->objectName().at(0)];
        HolonSidebar *sidebar = sidebarObjects.sidebar;
        SidebarButton *button = sidebarObjects.button;
        HolonSidebarArea *prev = sidebarObjects.area;

        if (button->isChecked())
            prev->hide();
        else
            button->setChecked(true);

        if (isVisible())
        {
            for (SidebarRelatedObjects value : qAsConst(desktop()->d_ptr->sidebarRelatedObjects))
            {
                if (value.sidebar == currentWidget())
                    value.button->setChecked(false);
            }
        }
        else
            show();

        QStackedWidget::addWidget(sidebar);
        setCurrentWidget(sidebar);

        button->area = objectName();
        sidebarObjects.area = this;

        return true;
    }

    return false;
}

void HolonSidebarArea::hide()
{
    QWidget::hide();
    setValue("hidden", true);
}

HolonDesktop *HolonSidebarArea::desktop() const
{
    return qobject_cast<HolonSplitted*>(parent())->desktop();
}

void HolonSidebarArea::setCurrentIndex(int i)
{
    QStackedWidget::setCurrentIndex(i);
    setStateIndex(i);
}

void HolonSidebarArea::setCurrentWidget(HolonSidebar *sidebar)
{
    QStackedWidget::setCurrentWidget(sidebar);
    setStateIndex(currentIndex());
}

void HolonSidebarArea::show()
{
    QWidget::show();
    setValue("hidden", false);
}
