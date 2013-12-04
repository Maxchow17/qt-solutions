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

#include <stdlib.h>
#include <string.h>
#include <QtCore/QFile>
#include <QtXml/QtXml>
#include <qtlibxmlreader.h>
#include <stdio.h>
#include <QtCore/QTextStream>

#include "parser.h"

static QTextStream qout(stdout, QIODevice::WriteOnly);
static QTextStream qerr(stderr, QIODevice::WriteOnly);

static void usage()
{
    qerr << "Usage: example [-libxml] [-qtxml] [-noqxmlinputsource] [-<feature>] <file.xml>\n";
    qerr << "Features: NOENT, RECOVER\n";
    exit(1);
}

int main(int argc, const char **argv)
{
    if (argc < 2)
        usage();

    QString file_name;
    QStringList feature_list;
    bool use_libxml = true, use_qtxml = true, noqxmlinputsource = false;

#define CHECK_FEATURE(ftr) \
    else if (arg == QString::fromLatin1("-" #ftr)) \
        feature_list.append(QString::fromLatin1("http://qtsoftware.com/xml/features/LIBXML2_" #ftr));

    for (int i = 1; i < argc; ++i) {
        QString arg = argv[i];
        if (arg == "-libxml")
            use_qtxml = false;
        else if (arg == "-qtxml")
            use_libxml = false;
        else if (arg == "-noqxmlinputsource")
            noqxmlinputsource= true;
        CHECK_FEATURE(NOENT)
        CHECK_FEATURE(RECOVER)
        else if (file_name.isNull())
            file_name = arg;
        else
            usage();
    }

#undef CHECK_FEATURE

    if (file_name.isNull())
        usage();

    int result = 0;

    if (use_libxml) {
        QtLibxmlReader *libxml_reader = new QtLibxmlReader;
        for (int i = 0; i < feature_list.size(); ++i)
            libxml_reader->setFeature(feature_list[i], true);
        Parser parser(libxml_reader);
        bool ok;
        if (noqxmlinputsource) {
            ok = parser.parse(file_name);
        } else {
            QFile file(file_name);
            if (file.open(QIODevice::ReadOnly)) {
                ok = parser.parse(&file);
            } else {
                qerr << "Could not open \"" << file_name.toLatin1() << "\"\n";
                ok = false;
            }
        }

        if (!ok)
            result = 1;

        qerr << "QtLibxmlReader returned " << ok << "\n";
        qout << parser.result() << "\n";
    }

    if (use_qtxml) {
        Parser parser(new QXmlSimpleReader);
        QFile file(file_name);
        bool ok;
        if (file.open(QIODevice::ReadOnly)) {
            ok = parser.parse(&file);
        } else {
            qerr << "Could not open \"" << file_name.toLatin1() << "\"\n";
            ok = false;
        }

        if (!ok)
            result = 1;

        qerr << "QXmlSimpleReader returned " << ok << "\n";
        qout << parser.result() << "\n";
    }

    return result;
};
