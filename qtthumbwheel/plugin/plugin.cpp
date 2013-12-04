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

#include <QtDesigner/QDesignerCustomWidgetInterface>
#include <QtCore/QtPlugin>
#include <QtGui/QPixmap>

#include "../src/qtthumbwheel.h"
#include "plugin.h"
#include "qtthumbwheel.xpm"

QtThumbWheelPlugin::QtThumbWheelPlugin(QWidget *parent)
    : QObject(parent)
{
    m_initialized = false;
}

QString QtThumbWheelPlugin::name() const
{
    return QLatin1String("QtThumbWheel");
}

QWidget *QtThumbWheelPlugin::createWidget(QWidget *parent)
{
    return new QtThumbWheel(parent);
}

QString QtThumbWheelPlugin::includeFile() const
{
    return "qtthumbwheel.h";
}

void QtThumbWheelPlugin::initialize(QDesignerFormEditorInterface *core)
{
    Q_UNUSED(core);
    if(m_initialized)
        return;
    m_initialized = true;
}

bool QtThumbWheelPlugin::isContainer() const
{
    return false;
}

bool QtThumbWheelPlugin::isInitialized() const
{
    return m_initialized;
}

QString QtThumbWheelPlugin::group() const
{
    return "Qt Solutions Widgets";
}

QString QtThumbWheelPlugin::toolTip() const
{
    return "A QtThumbWheel widget";
}

QString QtThumbWheelPlugin::whatsThis() const
{
    return "A QtThumbWheel widget";
}

QIcon QtThumbWheelPlugin::icon() const
{
    return QIcon(QPixmap(const_cast<const char **>(qtthumbwheel_xpm)));
}

QString QtThumbWheelPlugin::domXml() const
{
    return QLatin1String("<widget class=\"QtThumbWheel\" name=\"thumbwheel\"></widget>");
}

Q_EXPORT_PLUGIN2(qtthumbwheelplugin,QtThumbWheelPlugin)
