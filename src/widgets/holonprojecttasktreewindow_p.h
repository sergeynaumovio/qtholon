// Copyright (C) 2024 Sergey Naumov <sergey@naumov.io>
// SPDX-License-Identifier: 0BSD

#ifndef HOLONPROJECTTASKTREEWINDOW_P_H
#define HOLONPROJECTTASKTREEWINDOW_P_H

class HolonDesktop;
class HolonProjectTaskTreeWindow;
class HolonTaskFolderModel;
class HolonOpenTaskTreeModel;
class QLoaderSettings;
class QTreeView;
class QWidget;

class HolonProjectTaskTreeWindowPrivate
{
public:
    HolonProjectTaskTreeWindow *const q_ptr;
    QLoaderSettings *const settings;
    HolonDesktop *const desktop;
    HolonTaskFolderModel *const model;
    QTreeView *view{};
    HolonOpenTaskTreeModel *openTaskTreeModel;

    HolonProjectTaskTreeWindowPrivate(HolonProjectTaskTreeWindow *q = nullptr,
                                      QLoaderSettings *s = nullptr,
                                      HolonDesktop *desk = nullptr);

    QWidget *widget();
    void setOpenTaskTreeModel(HolonOpenTaskTreeModel *openTaskTreeModel);
};

#endif // HOLONPROJECTTASKTREEWINDOW_P_H
