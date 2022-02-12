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
#include "holonsidebar.h"

#include <QMainWindow>
#include <QStackedWidget>
#include <QDockWidget>
#include <QLabel>
#include <QBoxLayout>
#include <QShortcut>

#define D(Class) Class##PrivateData *const d = static_cast<Class##PrivateData*>(d_ptr.get())

struct Desktop
{
    HolonDesktopPrivate *const d_ptr;
    HolonDesktop *const q_ptr;
};

class HolonDesktopPrivate
{
protected:
    HolonDesktop *const q_ptr;

    HolonDesktopPrivate(HolonDesktop *q)
    :   q_ptr(q)
    { }

public:
    virtual bool isSidebarAreasMovable() const = 0;
    virtual void setSidebarAreasMovable(bool movable) = 0;

    QString sidebarAreasMovableShortcut() const
    {
        return q_ptr->value("sidebarAreasMovableShortcut").toString();
    }

    void stageState(const QByteArray &state)
    {
        q_ptr->setValue("mainWindowState", state);
    }
};

class HolonSidebarAreaTitleBar : public QWidget
{
public:
    HolonSidebarAreaTitleBar(HolonDesktop *desktop, QDockWidget *parent)
    :   QWidget(parent)
    {
        setStyleSheet(desktop->titleBarStyleSheet());
        setLayout(new QHBoxLayout(this));
        {
            QLabel *label = new QLabel("", this);
            {
                label->setFixedHeight(desktop->titleBarHeight());
                layout()->addWidget(label);
            }
        }
    }
};

class HolonSidebarArea : public QDockWidget
{
    struct Data
    {
        Desktop desktop;
        QMainWindow *mainWindow;
        bool sidebarAreasAdded{};

        struct
        {
            HolonSidebarAreaTitleBar *visible;
            QWidget *hidden;

        } titlebar;

        QStackedWidget *widget;
        QWidget *currentWidget;

        Data(Desktop ptrs, QMainWindow *parent)
        :   desktop(ptrs),
            mainWindow(parent)
        { }

    } d;

protected:
    void resizeEvent(QResizeEvent *) override
    {
        if (d.sidebarAreasAdded)
            d.desktop.d_ptr->stageState(d.mainWindow->saveState());
    }

public:
    HolonSidebarArea(const QString &name, Desktop desktop, QMainWindow *parent)
    :   QDockWidget(parent),
        d(desktop, parent)
    {
        d.titlebar.hidden = new QWidget(this);
        d.widget = new QStackedWidget(this);

        QLabel *label = new QLabel(this);
        {
            label->setText(name);
            QFont font("Arial", 20, QFont::Bold);
            label->setFont(font);
            label->setAlignment(Qt::AlignCenter);
            d.widget->addWidget(label);
        }

        setFeatures(QDockWidget::DockWidgetMovable);
        setObjectName(name);
        setTitleBarWidget(d.titlebar.hidden);
        setWidget(d.widget);
    }

    void addSidebar(HolonSidebar *sidebar)
    {
        d.widget->addWidget(sidebar);
        d.widget->setCurrentWidget(sidebar);
    }

    void setSidebarAreasAdded(bool added)
    {
        d.sidebarAreasAdded = added;
    }

    void showTitleBarWidget(bool show)
    {
        if (show)
        {
            d.currentWidget = d.widget->currentWidget();
            d.widget->setCurrentIndex(0);
            setTitleBarWidget(d.titlebar.visible = new HolonSidebarAreaTitleBar(d.desktop.q_ptr, this));
            d.titlebar.hidden->deleteLater();
        }
        else
        {
            d.widget->setCurrentWidget(d.currentWidget);
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
    HolonMainWindow(Desktop desktop, QWidget *parent)
    :   workspaces(new QStackedWidget(this))
    {
        setParent(parent);

        setDockOptions(QMainWindow::AnimatedDocks | QMainWindow::AllowNestedDocks);

        for (const QString &name : desktop.q_ptr->sidebarAreaList())
        {
            HolonSidebarArea *area = new HolonSidebarArea(name, desktop, this);
            areas.append(area);
            addDockWidget(Qt::LeftDockWidgetArea, area);
        }

        for (HolonSidebarArea *area : areas)
        {
            area->setSidebarAreasAdded(true);
            connect(area, &QDockWidget::dockLocationChanged, desktop.q_ptr, [desktop, this]
            {
                desktop.d_ptr->stageState(saveState());
            });
        }

        setCentralWidget(workspaces);
        workspaces->addWidget(new QLabel("Workspaces", workspaces));

        QShortcut *shortcut = new QShortcut(QKeySequence(desktop.d_ptr->sidebarAreasMovableShortcut()), this);
        connect(shortcut, &QShortcut::activated, this, [desktop]()
        {
            bool movable = desktop.d_ptr->isSidebarAreasMovable();
            desktop.d_ptr->setSidebarAreasMovable(!movable);
        });
    }

    void addSidebar(HolonSidebar *sidebar)
    {
        for (HolonSidebarArea *area : areas)
        {
            if (area->objectName() == sidebar->sidebarArea())
            {
                area->addSidebar(sidebar);
                break;
            }
        }
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

    void setMainWindow(HolonMainWindow **widget, Desktop desktop, QWidget *parent)
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

    void setDesktopLayout(Desktop desktop)
    {
        setVBoxLayout(d.desktop = desktop.q_ptr);
        setVBoxLayout(&d.screen, "Screen", d.desktop);
        {
            setVBoxLayout(&d.top, "ScreenTop", d.screen);
            setHBoxLayout(&d.middle, "ScreenMiddle", d.screen);
            {
                setHBoxLayout(&d.left, "ScreenLeft", d.middle);
                setMainWindow(&d.center, desktop, d.middle);
                setHBoxLayout(&d.right, "ScreenRight", d.middle);
            }
            setVBoxLayout(&d.bottom, "ScreenBottom", d.screen);
        }
    }
};

class HolonDesktopPrivateData : public HolonDesktopPrivate
{
public:
    HolonDesktopLayout desktopLayout;
    QStringList sidebarAreaList;
    bool sidebarAreasMovable{};
    QList<QChar> sidebarList;
    int titleBarHeight;
    QString titleBarStyleSheet;
    QList<HolonSidebar*> sidebars;

    HolonDesktopPrivateData(HolonDesktop *q)
    :   HolonDesktopPrivate(q)
    { }

    void addTaskbar(HolonTaskbar *taskbar)
    {
        desktopLayout.addTaskbar(taskbar);
    }

    bool isSidebarAreasMovable() const override
    {
        return sidebarAreasMovable;
    }

    void setDesktopLayout()
    {
        desktopLayout.setDesktopLayout({this, q_ptr});
    }

    void setSidebarAreasMovable(bool movable) override
    {
        sidebarAreasMovable = movable;
        desktopLayout.mainWindow()->showTitleBarWidgets(movable);
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
    d_ptr(new HolonDesktopPrivateData(this))
{
    D(HolonDesktop);

    if (!parent)
        show();

    if (!contains("sidebarAreaList"))
    {
        emitError("sidebarAreaList property is not set");
        return;
    }

    bool validSidebarAreaList = [=, this]()
    {
        d->sidebarAreaList = value("sidebarAreaList").toStringList();
        for (const QString &string : d->sidebarAreaList)
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
        d->sidebarList = value("sidebarList").value<QCharList>();
        if (!d->sidebarList.size())
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

    d->titleBarHeight = value("titleBarHeight", 10).toInt();
    d->titleBarStyleSheet = value("titleBarStyleSheet").toString();

    d->setDesktopLayout();

    d->desktopLayout.mainWindow()->restoreState(value("mainWindowState").toByteArray());
}

HolonDesktop::~HolonDesktop()
{ }

bool HolonDesktop::addSidebar(HolonSidebar *sidebar)
{
    D(HolonDesktop);
    if (!d->sidebarList.contains(sidebar->sidebar()) ||
        !d->sidebarAreaList.contains(sidebar->sidebarArea()) ||

        std::find_if(d->sidebars.begin(), d->sidebars.end(),
                     [sidebar](HolonSidebar *current)
                     { return current->sidebar() == sidebar->sidebar(); }) != d->sidebars.end())
    {
        return false;
    }

    d->sidebars.append(sidebar);
    d->desktopLayout.mainWindow()->addSidebar(sidebar);

    return true;
}

void HolonDesktop::addTask(HolonTask* /*task*/)
{

}

void HolonDesktop::addTaskbar(HolonTaskbar *taskbar)
{
    D(HolonDesktop);
    d->addTaskbar(taskbar);
}

QStringList HolonDesktop::sidebarAreaList() const
{
    D(HolonDesktop);
    return d->sidebarAreaList;
}

QCharList HolonDesktop::sidebarList() const
{
    D(HolonDesktop);
    return d->sidebarList;
}

int HolonDesktop::titleBarHeight() const
{
    D(HolonDesktop);
    return d->titleBarHeight;
}

QString HolonDesktop::titleBarStyleSheet() const
{
    D(HolonDesktop);
    return d->titleBarStyleSheet;
}
