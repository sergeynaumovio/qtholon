/****************************************************************************
**
** Copyright (C) 2022 Sergey Naumov
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
#include "holontaskbar.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QShortcut>

class HolonSidebarAreaTitleBar : public QWidget
{
public:
    HolonSidebarAreaTitleBar(HolonDesktop *desktop, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop->barStyleSheet());
        setLayout(new QHBoxLayout(this));
        layout()->addWidget(new QLabel("", this));
    }
};

class HolonSidebarArea : public QDockWidget
{
    struct
    {
        HolonDesktop *desktop;

        struct
        {
            HolonSidebarAreaTitleBar *visible;
            QWidget *hidden;

        } titlebar;

        QStackedWidget *widget;

    } d;

public:
    HolonSidebarArea(const QString &name, HolonDesktop *desktop, QMainWindow *parent)
    :   QDockWidget(parent)
    {
        d.desktop = desktop;
        d.titlebar.hidden = new QWidget(this);
        d.widget = new QStackedWidget(this);

        setFeatures(QDockWidget::DockWidgetMovable);

        QLabel *label = new QLabel(this);
        {
            label->setText(name);
            QFont font("Arial", 20, QFont::Bold);
            label->setFont(font);
            label->setAlignment(Qt::AlignCenter);
            d.widget->addWidget(label);
        }

        setTitleBarWidget(d.titlebar.hidden);
        setWidget(d.widget);
    }

    void showTitleBarWidget(bool show)
    {
        if (show)
        {
            setTitleBarWidget(d.titlebar.visible = new HolonSidebarAreaTitleBar(d.desktop, this));
            d.titlebar.hidden->deleteLater();
        }
        else
        {
            setTitleBarWidget(d.titlebar.hidden = new QWidget(this));
            d.titlebar.visible->deleteLater();
        }
    }
};

class HolonMainWindow : public QMainWindow
{
    QList<HolonSidebarArea*> areas;
    QStackedWidget *workspaces;

public:
    HolonMainWindow(HolonDesktop *desktop, QWidget *parent)
    :   workspaces(new QStackedWidget(this))
    {
        setParent(parent);

        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

        for (const QString &name : desktop->sidebarAreaList())
        {
            HolonSidebarArea *area = new HolonSidebarArea(name, desktop, this);
            areas.append(area);
            addDockWidget(Qt::LeftDockWidgetArea, area);
        }

        setCentralWidget(workspaces);
        workspaces->addWidget(new QLabel("Workspaces", workspaces));

        QShortcut *shortcut = new QShortcut(QKeySequence(desktop->sidebarAreasMovableShortcut()), this);
        connect(shortcut, &QShortcut::activated, this, [desktop]()
        {
            bool movable = desktop->isSidebarAreasMovable();
            desktop->setSidebarAreasMovable(!movable);
        });
    }

    void showTitleBarWidgets(bool show)
    {
        for (HolonSidebarArea *area : areas)
            area->showTitleBarWidget(show);
    }
};

class HolonDesktopLayout
{
    struct
    {
        HolonDesktop *desktop;
        QWidget *screen;
        QWidget *top;
        QWidget *middle;
        QWidget *bottom;
        QWidget *left;
        QWidget *right;
        HolonMainWindow *center;

    } d;

    void addWidget(QWidget *widget, QWidget *parent)
    {
        parent->layout()->addWidget(widget);
    }

    void setHBoxLayout(QWidget **widget, const char *name, QWidget *parent)
    {
        *widget = new QWidget(parent);
        (*widget)->setObjectName(name);
        (*widget)->setLayout(new QHBoxLayout(*widget));
        (*widget)->layout()->setContentsMargins({});
        (*widget)->layout()->setSpacing(0);
        parent->layout()->addWidget(*widget);
    }

    void setMainWindow(HolonMainWindow **widget, HolonDesktop *desktop, QWidget *parent)
    {
        *widget = new HolonMainWindow(desktop, parent);
        (*widget)->setObjectName("ScreenCenter");
        parent->layout()->addWidget(*widget);
    }

    void setVBoxLayout(HolonDesktop *desktop)
    {
        desktop->setLayout(new QVBoxLayout(desktop));
        desktop->layout()->setContentsMargins({});
        desktop->layout()->setSpacing(0);
    }

    void setVBoxLayout(QWidget **widget, const char *name, QWidget *parent)
    {
        *widget = new QWidget(parent);
        (*widget)->setObjectName(name);
        (*widget)->setLayout(new QVBoxLayout(*widget));
        (*widget)->layout()->setContentsMargins({});
        (*widget)->layout()->setSpacing(0);
        parent->layout()->addWidget(*widget);
    }

public:
    void addTaskbar(HolonTaskbar *taskbar)
    {
        HolonTaskbar::Area area = taskbar->area();
        QWidget *parent{};

        switch (area) {
        case HolonTaskbar::Top: parent = d.top; break;
        case HolonTaskbar::Bottom: parent = d.bottom; break;
        case HolonTaskbar::Left: parent = d.left; break;
        case HolonTaskbar::Right: parent = d.right; break;
        }

        addWidget(taskbar, parent);
    }

    HolonMainWindow *mainWindow() { return d.center; }

    void setDesktopLayout(HolonDesktop *desktop)
    {
        setVBoxLayout(d.desktop = desktop);
        setVBoxLayout(&d.screen, "Screen", d.desktop);
        {
            setVBoxLayout(&d.top, "ScreenTop", d.screen);
            setHBoxLayout(&d.middle, "ScreenMiddle", d.screen);
            {
                setHBoxLayout(&d.left, "ScreenLeft", d.middle);
                setMainWindow(&d.center, d.desktop, d.middle);
                setHBoxLayout(&d.right, "ScreenRight", d.middle);
            }
            setVBoxLayout(&d.bottom, "ScreenBottom", d.screen);
        }
    }
};

class HolonDesktopPrivate
{
public:
    HolonDesktop *const q_ptr;
    HolonDesktopLayout desktopLayout;
    QStringList sidebarAreaList;
    bool sidebarAreasMovable{};
    QList<QChar> sidebarList;
    QString barStyleSheet;

    HolonDesktopPrivate(HolonDesktop *q)
    :   q_ptr(q)
    { }

    void addTaskbar(HolonTaskbar *taskbar)
    {
        desktopLayout.addTaskbar(taskbar);
    }

    void setDesktopLayout()
    {
        desktopLayout.setDesktopLayout(q_ptr);
    }
};

void HolonDesktop::closeEvent(QCloseEvent *)
{
    hide();
    deleteLater();
}

void HolonDesktop::resizeEvent(QResizeEvent *)
{
    emit sizeChanged(size());
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
    if (!parent)
        show();

    if (!contains("sidebarAreaList"))
    {
        emitError("sidebarAreaList property is not set");
        return;
    }

    bool validSidebarAreaList = [this]()
    {
        d_ptr->sidebarAreaList = value("sidebarAreaList").toStringList();
        for (const QString &string : d_ptr->sidebarAreaList)
        {
            if (string.isEmpty())
                return false;
        }
        return true;
    }();

    if (!validSidebarAreaList)
    {
        emitError("sidebarArea name is not set");
        return;
    }

    if (contains("sidebarList"))
    {
        d_ptr->sidebarList = value("sidebarList").value<QCharList>();
        if (!d_ptr->sidebarList.size())
        {
            emitError("sidebarList item is not a char");
            return;
        }
    }
    else
    {
        emitError("sidebarList property is not set");
        return;
    }

    d_ptr->barStyleSheet = value("barStyleSheet").toString();

    d_ptr->setDesktopLayout();
}

HolonDesktop::~HolonDesktop()
{ }

bool HolonDesktop::addSidebar(HolonSidebar* /*sidebar*/)
{
    return true;
}

void HolonDesktop::addTask(HolonTask* /*task*/)
{

}

void HolonDesktop::addTaskbar(HolonTaskbar *taskbar)
{
    d_ptr->addTaskbar(taskbar);
}

QString HolonDesktop::barStyleSheet() const
{
    return d_ptr->barStyleSheet;
}

bool HolonDesktop::isSidebarAreasMovable() const
{
    return d_ptr->sidebarAreasMovable;
}

void HolonDesktop::setSidebarAreasMovable(bool movable)
{
    d_ptr->sidebarAreasMovable = movable;
    d_ptr->desktopLayout.mainWindow()->showTitleBarWidgets(movable);
}

QStringList HolonDesktop::sidebarAreaList() const
{
    return d_ptr->sidebarAreaList;
}

QString HolonDesktop::sidebarAreasMovableShortcut() const
{
    return value("sidebarAreasMovableShortcut").toString();
}

QCharList HolonDesktop::sidebarList() const
{
    return d_ptr->sidebarList;
}
