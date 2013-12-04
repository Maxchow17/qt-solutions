#include "privateqt_mac.h"
#import <AppKit/AppKit.h>
#include <QtCore/QVarLengthArray>

QString QtCFString::toQString(CFStringRef str)
{
    if(!str)
        return QString();
    CFIndex length = CFStringGetLength(str);
    const UniChar *chars = CFStringGetCharactersPtr(str);
    if (chars)
        return QString(reinterpret_cast<const QChar *>(chars), length);

    QVarLengthArray<UniChar> buffer(length);
    CFStringGetCharacters(str, CFRangeMake(0, length), buffer.data());
    return QString(reinterpret_cast<const QChar *>(buffer.constData()), length);
}

QtCFString::operator QString() const
{
    if (string.isEmpty() && type)
        const_cast<QtCFString*>(this)->string = toQString(type);
    return string;
}

CFStringRef QtCFString::toCFStringRef(const QString &string)
{
    return CFStringCreateWithCharacters(0, reinterpret_cast<const UniChar *>(string.unicode()),
                                        string.length());
}

QtCFString::operator CFStringRef() const
{
    if (!type)
        const_cast<QtCFString*>(this)->type = toCFStringRef(string);
    return type;
}

QtMacCocoaAutoReleasePool::QtMacCocoaAutoReleasePool()
{
    NSApplicationLoad();
    pool = (void*)[[NSAutoreleasePool alloc] init];
}

QtMacCocoaAutoReleasePool::~QtMacCocoaAutoReleasePool()
{
    [(NSAutoreleasePool*)pool release];
}
