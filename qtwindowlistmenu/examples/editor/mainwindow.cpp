/****************************************************************************
**
** Copyright (c) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of a Qt Solutions component.
**
** Commercial Usage
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Solutions Commercial License Agreement provided
** with the Software or, alternatively, in accordance with the terms
** contained in a written agreement between you and Digia.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain
** additional rights. These rights are described in the Digia Qt LGPL
** Exception version 1.1, included in the file LGPL_EXCEPTION.txt in this
** package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
** Please note Third Party Software included with Qt Solutions may impose
** additional restrictions and it is the user's responsibility to ensure
** that they have met the licensing requirements of the GPL, LGPL, or Qt
** Solutions Commercial license and the relevant license of the Third
** Party Software they are using.
**
** If you are unsure which license is appropriate for your use, please
** contact Digia at http://www.qt-project.org/legal.
**
****************************************************************************/


#include "mainwindow.h"
#include "mdichild.h"
#include "menueditor.h"
#include <qtwindowlistmenu.h>

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMdiSubWindow>
#include <QtGui/QCloseEvent>

#include <QDebug> //######################3

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);
    setCentralWidget(mdiArea);

    QtWindowListMenu *winMenu = new QtWindowListMenu(menuBar());
    winMenu->attachToMdiArea(mdiArea);
    menuBar()->insertMenu(menuHelp->menuAction(), winMenu);

    connect(mdiArea, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateEditActions()));
    updateEditActions();

    MenuEditor *menuEditor = new MenuEditor(mdiArea, winMenu, this);
    addDockWidget(Qt::RightDockWidgetArea, menuEditor);
    menuEditor->hide();
    menuEdit->addAction(menuEditor->toggleViewAction());
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    mdiArea->closeAllSubWindows();
    if (mdiArea->subWindowList().size())
        event->ignore();
    else
        event->accept();
}


void MainWindow::on_actionNew_triggered()
{
    MdiChild *child = createMdiChild();
    child->newFile();
    child->show();
}

void MainWindow::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty()) {
        QMdiSubWindow *existing = findMdiChild(fileName);
        if (existing) {
            mdiArea->setActiveSubWindow(existing);
            return;
        }

        MdiChild *child = createMdiChild();
        if (child->loadFile(fileName)) {
            statusBar()->showMessage(tr("File loaded"), 2000);
            child->show();
        } else {
            child->close();
        }
    }
}

void MainWindow::on_actionSave_triggered()
{
    if (activeMdiChild() && activeMdiChild()->save())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::on_actionSaveAs_triggered()
{
    if (activeMdiChild() && activeMdiChild()->saveAs())
        statusBar()->showMessage(tr("File saved"), 2000);
}

void MainWindow::on_actionCut_triggered()
{
    if (activeMdiChild())
        activeMdiChild()->cut();
}

void MainWindow::on_actionCopy_triggered()
{
    if (activeMdiChild())
        activeMdiChild()->copy();
}

void MainWindow::on_actionPaste_triggered()
{
    if (activeMdiChild())
        activeMdiChild()->paste();
}

void MainWindow::on_actionAbout_triggered()
{
    QMessageBox::about(this, tr("About MDI"),
                       tr("This example demonstrates how to use the"
                          "QtWindowListMenu component of Qt Solutions in an MDI"
                          "application."));
}

void MainWindow::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}



MdiChild *MainWindow::createMdiChild()
{
    MdiChild *child = new MdiChild;
    mdiArea->addSubWindow(child);

    connect(child, SIGNAL(copyAvailable(bool)), actionCut, SLOT(setEnabled(bool)));
    connect(child, SIGNAL(copyAvailable(bool)), actionCopy, SLOT(setEnabled(bool)));

    return child;
}


MdiChild *MainWindow::activeMdiChild()
{
    if (QMdiSubWindow *activeSubWindow = mdiArea->activeSubWindow())
        return qobject_cast<MdiChild *>(activeSubWindow->widget());
    return 0;
}

QMdiSubWindow *MainWindow::findMdiChild(const QString &fileName)
{
    QString canonicalFilePath = QFileInfo(fileName).canonicalFilePath();

    foreach (QMdiSubWindow *window, mdiArea->subWindowList()) {
        MdiChild *mdiChild = qobject_cast<MdiChild *>(window->widget());
        if (mdiChild->currentFile() == canonicalFilePath)
            return window;
    }
    return 0;
}



void MainWindow::updateEditActions()
{
    bool hasSelection = (activeMdiChild() &&
                         activeMdiChild()->textCursor().hasSelection());
    actionCut->setEnabled(hasSelection);
    actionCopy->setEnabled(hasSelection);
}
