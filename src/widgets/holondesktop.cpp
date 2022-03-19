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
#include <QRegularExpression>
#include <QLoaderTree>

class HolonMainWindow;
class HolonDesktopLayout;

class HolonDesktopLayoutData
{
protected:
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

public:
    HolonDesktopLayout *operator->() { return reinterpret_cast<HolonDesktopLayout*>(this); }
};

struct Desktop
{
    HolonDesktopPrivate *const d_ptr;
    HolonDesktop *const q_ptr;
};

class HolonDesktopPrivate
{
public:
    HolonDesktop *const q_ptr;
    QString error;
    const QString buttonStyleSheet;
    const QString menuStyleSheet;
    const QRegularExpression borderWidth{"^QWidget\\s*{[^}]*border:[^};]*(?<px>\\d+)px[^}]*}$"};
    QLoaderData *data{};
    const int menuBorder;
    const int menuWidth;
    const QStringList sidebarAreaList;
    const QList<QChar> sidebarCharList;
    QList<HolonSidebar*> sidebarList;
    const int titleBarHeight;
    const QString titleBarStyleSheet;
    QList<HolonWindow *> windowList;
    HolonDesktopLayoutData desktopLayout;

    HolonDesktopPrivate(HolonDesktop *q)
    :   q_ptr(q),
        buttonStyleSheet(q->value("buttonStyleSheet").toString()),
        menuStyleSheet(q->value("menuStyleSheet").toString()),
        menuBorder([this]()
        {
            QRegularExpressionMatch match = borderWidth.match(menuStyleSheet);
            if (match.hasMatch())
                return match.captured("px").toInt();

            return 1;
        }()),
        menuWidth(q->value("menuWidth", 200).toInt()),
        sidebarAreaList(q->value("sidebarAreaList").toStringList()),
        sidebarCharList(q->value("sidebarList").value<QCharList>()),
        titleBarHeight(q->value("titleBarHeight", 10).toInt()),
        titleBarStyleSheet(q ->value("titleBarStyleSheet").toString())
    {
        if (!sidebarAreaList.size())
        {
            error = "sidebarAreaList property is not set";
            return;
        }

        bool validSidebarAreaList = [=, this]()
        {
            for (const QString &string : sidebarAreaList)
            {
                if (string.isEmpty())
                    return false;
            }
            return true;
        }();

        if (!validSidebarAreaList)
        {
            error = "sidebarArea name is not set";
            return;
        }

        if (!sidebarCharList.size())
        {
            error = "sidebarList item is not set or not a char";
            return;
        }
    }

    QString sidebarAreaMoveShortcut() const
    {
        return q_ptr->value("sidebarAreaMoveShortcut").toString();
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
            layout()->setContentsMargins({});
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
    bool visibleTitleBar{};
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

        QShortcut *shortcut = new QShortcut(QKeySequence(desktop.d_ptr->sidebarAreaMoveShortcut()), this);
        connect(shortcut, &QShortcut::activated, this, [this]()
        {
            visibleTitleBar = !visibleTitleBar;
            for (HolonSidebarArea *area : areas)
                area->showTitleBarWidget(visibleTitleBar);
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
};

class HolonDesktopLayout : public HolonDesktopLayoutData
{
    void addWidget(QWidget *widget, QWidget *parent)
    {
        parent->layout()->addWidget(widget);
    }

    void setHBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
    {
        widget = new QWidget(parent);
        widget->setObjectName(name);
        widget->setLayout(new QHBoxLayout(widget));
        widget->layout()->setContentsMargins({});
        widget->layout()->setSpacing(0);
        parent->layout()->addWidget(widget);
    }

    void setMainWindow(HolonMainWindow *&widget, Desktop desktop, QWidget *parent)
    {
        widget = new HolonMainWindow(desktop, parent);
        widget->setObjectName("ScreenCenter");
        parent->layout()->addWidget(widget);
    }

    void setVBoxLayout(HolonDesktop *desktop)
    {
        desktop->setLayout(new QVBoxLayout(desktop));
        desktop->layout()->setContentsMargins({});
        desktop->layout()->setSpacing(0);
    }

    void setVBoxLayout(QWidget *&widget, const char *name, QWidget *parent)
    {
        widget = new QWidget(parent);
        widget->setObjectName(name);
        widget->setLayout(new QVBoxLayout(widget));
        widget->layout()->setContentsMargins({});
        widget->layout()->setSpacing(0);
        parent->layout()->addWidget(widget);
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
        setVBoxLayout(d.screen, "Screen", d.desktop);
        {
            setVBoxLayout(d.top, "ScreenTop", d.screen);
            setHBoxLayout(d.middle, "ScreenMiddle", d.screen);
            {
                setHBoxLayout(d.left, "ScreenLeft", d.middle);
                setMainWindow(d.center, desktop, d.middle);
                setHBoxLayout(d.right, "ScreenRight", d.middle);
            }
            setVBoxLayout(d.bottom, "ScreenBottom", d.screen);
        }
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
    if (d_ptr->error.size())
    {
        emitError(d_ptr->error);
        return;
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

    if (!parent)
        show();

    d_ptr->desktopLayout->setDesktopLayout({d_ptr.get(), this});
    d_ptr->desktopLayout->mainWindow()->restoreState(value("mainWindowState").toByteArray());
}

HolonDesktop::~HolonDesktop()
{ }

bool HolonDesktop::addSidebar(HolonSidebar *sidebar)
{
    if (!d_ptr->sidebarCharList.contains(sidebar->sidebar()) ||
        !d_ptr->sidebarAreaList.contains(sidebar->sidebarArea()) ||

        std::find_if(d_ptr->sidebarList.begin(), d_ptr->sidebarList.end(),
                     [sidebar](HolonSidebar *current)
                     { return current->sidebar() == sidebar->sidebar(); }) != d_ptr->sidebarList.end())
    {
        return false;
    }

    d_ptr->sidebarList.append(sidebar);
    d_ptr->desktopLayout->mainWindow()->addSidebar(sidebar);

    return true;
}

void HolonDesktop::addTask(HolonTask* /*task*/)
{

}

void HolonDesktop::addTaskbar(HolonTaskbar *taskbar)
{
    d_ptr->desktopLayout->addTaskbar(taskbar);
}

void HolonDesktop::addWindow(HolonWindow *window)
{
    d_ptr->windowList.append(window);
}

QString HolonDesktop::buttonStyleSheet() const
{
    return d_ptr->buttonStyleSheet;
}

QLoaderData *HolonDesktop::data() const
{
    return d_ptr->data;
}

int HolonDesktop::menuBorder() const
{
    return d_ptr->menuBorder;
}

QString HolonDesktop::menuStyleSheet() const
{
    return d_ptr->menuStyleSheet;
}

int HolonDesktop::menuWidth() const
{
    return d_ptr->menuWidth;
}

void HolonDesktop::setData(QLoaderData *data)
{
    if (d_ptr->data)
        emitError("data already set");
    else
        d_ptr->data = data;
}

HolonSidebar *HolonDesktop::sidebar(QChar /*chr*/) const
{
    return nullptr;
}

QStringList HolonDesktop::sidebarAreaList() const
{
    return d_ptr->sidebarAreaList;
}

QCharList HolonDesktop::sidebarList() const
{
    return d_ptr->sidebarCharList;
}

int HolonDesktop::titleBarHeight() const
{
    return d_ptr->titleBarHeight;
}

QString HolonDesktop::titleBarStyleSheet() const
{
    return d_ptr->titleBarStyleSheet;
}

const QList<HolonWindow *> &HolonDesktop::windowList() const
{
    return d_ptr->windowList;
}
