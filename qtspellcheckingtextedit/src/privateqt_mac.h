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

#ifndef MAC_PRIVATE_H
#define MAC_PRIVATE_H

#include <CoreFoundation/CoreFoundation.h>
#include <QtCore/QString>

template <typename T>
class QtCFType
{
public:
    inline QtCFType(const T &t = 0) : type(t) {}
    inline QtCFType(const QtCFType &helper) : type(helper.type) {
        if (type) CFRetain(type);
    }
    inline ~QtCFType() {
        if (type) CFRelease(type);
    }
    inline operator T() {
        return type;
    }
    inline QtCFType operator =(const QtCFType &helper)
    {
        if (helper.type)
            CFRetain(helper.type);
        CFTypeRef type2 = type;
        type = helper.type;
        if (type2)
            CFRelease(type2);
        return *this;
    }
    inline T *operator&() {
        return &type;
    }
    static QtCFType constructFromGet(const T &t)
    {
        CFRetain(t);
        return QtCFType<T>(t);
    }
protected:
    T type;
};

class QtCFString : public QtCFType<CFStringRef>
{
public:
    inline QtCFString(const QString &str) : QtCFType<CFStringRef>(0), string(str) {}
    inline QtCFString(const CFStringRef cfstr = 0) : QtCFType<CFStringRef>(cfstr) {}
    inline QtCFString(const QtCFType<CFStringRef> &other) : QtCFType<CFStringRef>(other) {}
    operator QString() const;
    operator CFStringRef() const;
    static QString toQString(CFStringRef cfstr);
    static CFStringRef toCFStringRef(const QString &str);
private:
    QString string;
};

class QtMacCocoaAutoReleasePool
{
private:
    void *pool;
public:
    QtMacCocoaAutoReleasePool();
    ~QtMacCocoaAutoReleasePool();

    inline void *handle() const {
        return pool;
    }
};


#endif
