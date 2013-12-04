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

#include "menueditor.h"
#include <qtwindowlistmenu.h>

#include <QtGui/QApplication>
#include <QtGui/QFileDialog>
#include <QtGui/QMessageBox>
#include <QtGui/QMdiArea>

#include <QDebug> //######################3

MenuEditor::MenuEditor(QMdiArea *mdiArea, QtWindowListMenu *windowsMenu, QWidget *parent)
    : QDockWidget(parent), mdi(mdiArea), winMenu(windowsMenu)
{
    setupUi(this);

    iconItemCombo->setItemData(0, -2);  // "Current window"
    iconItemCombo->setItemData(1, -1);  // "Window default"
    iconItemCombo->setItemData(2, QtWindowListMenu::CloseAction);
    iconItemCombo->setItemData(3, QtWindowListMenu::CloseAllAction);
    iconItemCombo->setItemData(4, QtWindowListMenu::TileAction);
    iconItemCombo->setItemData(5, QtWindowListMenu::CascadeAction);
    iconItemCombo->setItemData(6, QtWindowListMenu::NextAction);
    iconItemCombo->setItemData(7, QtWindowListMenu::PrevAction);

    connect(mdi, SIGNAL(subWindowActivated(QMdiSubWindow *)),
            this, SLOT(updateIconControls()));
    updateIconControls();
}


void MenuEditor::on_iconItemCombo_activated()
{
    updateIconLabel();
    updateIconControls();
}


void MenuEditor::on_iconClearButton_clicked()
{
    switch(int val = iconItemCombo->itemData(iconItemCombo->currentIndex()).toInt()) {
    case -2:
        winMenu->setWindowIcon(mdi->currentSubWindow(), QIcon());
        break;
    case -1:
        winMenu->setDefaultIcon(QIcon());
        break;
    default:
        winMenu->standardAction((QtWindowListMenu::StandardAction)val)->setIcon(QIcon());
        break;
    }

    updateIconLabel();
}


void MenuEditor::on_iconChangeButton_clicked()
{
    QString iconFileName = QFileDialog::getOpenFileName(this, "Choose icon");
    if (iconFileName.isEmpty())
        return;

    QIcon icon(iconFileName);
    if (icon.pixmap(32).isNull()) {
        QMessageBox::warning(this, "Warning", "Could not read an icon from the file " + iconFileName);
        return;
    }

    switch(int val = iconItemCombo->itemData(iconItemCombo->currentIndex()).toInt()) {
    case -2:
        winMenu->setWindowIcon(mdi->currentSubWindow(), icon);
        break;
    case -1:
        winMenu->setDefaultIcon(icon);
        break;
    default:
        winMenu->standardAction((QtWindowListMenu::StandardAction)val)->setIcon(icon);
        break;
    }

    updateIconLabel();
}


void MenuEditor::updateIconLabel()
{
    QIcon icon;
    switch(int val = iconItemCombo->itemData(iconItemCombo->currentIndex()).toInt()) {
    case -2:
        icon = winMenu->windowIcon(mdi->currentSubWindow());
        break;
    case -1:
        icon = winMenu->defaultIcon();
        break;
    default:
        icon = winMenu->standardAction((QtWindowListMenu::StandardAction)val)->icon();
        break;
    }

    if (icon.isNull())
        iconLabel->setText("[NO ICON]");
    else
        iconLabel->setPixmap(icon.pixmap(32));
}


void MenuEditor::on_itemVisibleBox_toggled(bool checked)
{
    int val = iconItemCombo->itemData(iconItemCombo->currentIndex()).toInt();
    if (val < 0)
        return;

    winMenu->standardAction((QtWindowListMenu::StandardAction)val)->setVisible(checked);
}


void MenuEditor::updateIconControls()
{
    int val = iconItemCombo->itemData(iconItemCombo->currentIndex()).toInt();

    if (val == -2)  // "current window"
        updateIconLabel();

    bool canEditIcon = (mdi->activeSubWindow() != 0 || val != -2);
    iconClearButton->setEnabled(canEditIcon);
    iconChangeButton->setEnabled(canEditIcon);

    bool isStdItem = (val >= 0);
    bool isVisible = isStdItem ? winMenu->standardAction((QtWindowListMenu::StandardAction)val)->isVisible() : true;
    itemVisibleBox->setChecked(isVisible);
    itemVisibleBox->setEnabled(isStdItem);
}


