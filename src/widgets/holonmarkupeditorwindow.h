// Copyright (C) 2025 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONMARKUPEDITORWINDOW_H
#define HOLONMARKUPEDITORWINDOW_H

#include "holonabstracttaskwindow.h"

class HolonDesktop;
class HolonMarkupEditorWindowPrivate;
class HolonTaskStackedWindow;

class HolonMarkupEditorWindow : public HolonAbstractTaskWindow
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(HolonMarkupEditorWindow)

public:
    HolonMarkupEditorWindow(QLoaderSettings *settings, HolonAbstractTask *parent);
    HolonMarkupEditorWindow(QLoaderSettings *settings, HolonDesktop *parent);
    HolonMarkupEditorWindow(QLoaderSettings *settings, HolonTaskStackedWindow *parent);
    ~HolonMarkupEditorWindow();

    QWidget *centralWidget() override;
    QIcon icon() const override;
    bool isCopyable(QStringView to) const override;
    QWidget *settingsWidget() override;
    QString title() const override;
    QWidget *toolbarWidget() override;
};

#endif // HOLONMARKUPEDITORWINDOW_H
