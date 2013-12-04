#include "qtspellcheckingtextedit.h"
#include <CoreFoundation/CoreFoundation.h>
#include <Carbon/Carbon.h>
#import <AppKit/AppKit.h>
#include "privateqt_mac.h"
#include <QtCore/QList>
#include <QtGui/QMenu>
#include <QtGui/QAction>
#include <QtGui/QContextMenuEvent>
#include <QtGui/QApplication>
#include <QtGui/QClipboard>
#include <QtGui/QDesktopServices>
#include <QtCore/QUrl>
#include <QtGui/QSyntaxHighlighter>
#include <QtGui/QTextEdit>
#include <QtGui/QTextCursor>
#include <QtCore/QDebug>

#if (MAC_OS_X_VERSION_MAX_ALLOWED == MAC_OS_X_VERSION_10_4)
@interface NSSpellChecker (NotYetPublicMethods)
- (void)learnWord:(NSString *)word;
@end
#endif

struct _NSRange;
typedef struct _NSRange NSRange;
struct QtTextRange
{
    QtTextRange(const NSRange &nsrange);
    QtTextRange(int index, int length);
    bool operator==(const QtTextRange &other) const;
    int index;
    int length;
};

QtTextRange::QtTextRange(const NSRange &nsrange)
: index(nsrange.location), length(nsrange.length) { } 

QtTextRange::QtTextRange(int index, int length)
:index(index), length(length)
{ }

bool QtTextRange::operator==(const QtTextRange &other) const
{
    return (index == other.index && length == other.length);
}

QDebug operator<<(QDebug d, const QtTextRange &range)
{
    d << "index " << range.index << " lenght " << range.length;
    return d;
}

#ifdef __LP64__ 
# define TAG_TYPE NSInteger
#else
# define TAG_TYPE int
#endif
namespace QtSpellCheckerBridge
{
    // returns a list of ranges for all misspelt words in the text.
    QList<QtTextRange> spellingErrorIndexes(const QString& text, QTextDocument *document)
    {
        const QtMacCocoaAutoReleasePool pool;
        const QtCFString string(text);
        const int textLenght = text.length();
        
        int index = 0;
        QList<QtTextRange> ranges;
        while (index < textLenght) {
            const QtTextRange range = [[NSSpellChecker sharedSpellChecker] 
                                    checkSpellingOfString:(NSString *)(CFStringRef)string
                                    startingAt:index
                                    language: nil
                                    wrap : false
                                    inSpellDocumentWithTag : reinterpret_cast<TAG_TYPE>(document)
                                    wordCount : nil];
            const int rangeEnd = range.index + range.length;
            index = rangeEnd;
            if (range.index != INT_MAX)
                ranges.append(range);
        }
        return ranges;
    }

    // returns wether text contains no spelling errors.
    bool isSpeltCorrectly(const QString& text, QTextDocument *document)
    {
        return (spellingErrorIndexes(text, document).count() == 0);
    }

    // returns a list of suggestions for \a word.
    QStringList suggestions(const QString& word)
    {
        const QtMacCocoaAutoReleasePool pool;
        NSArray * const array  = [[NSSpellChecker sharedSpellChecker] guessesForWord : (NSString *)QtCFString::toCFStringRef(word)];

        QStringList suggestions;
        if (array == 0)
            return suggestions;

        for (unsigned int i = 0; i < [array count]; ++i)
            suggestions.append(QtCFString::toQString((CFStringRef)[array objectAtIndex: i]));
        
        return suggestions;
    }

    // Ignores a word when spell checking, for the given document only.
    void ignoreSpelling(const QString &word, QTextDocument *document)
    {
        const QtMacCocoaAutoReleasePool pool;
        [[NSSpellChecker sharedSpellChecker] 
            ignoreWord : (NSString *)QtCFString::toCFStringRef(word) 
            inSpellDocumentWithTag : reinterpret_cast<TAG_TYPE>(document)];
    }
    
    void learnSpelling(const QString &word)
    {
        [[NSSpellChecker sharedSpellChecker] learnWord: (NSString *)QtCFString::toCFStringRef(word)];
    }
}

class QtSpellCheckHighlighter : public QSyntaxHighlighter
{
public:
    QtSpellCheckHighlighter(QTextDocument *textDocument)
    : QSyntaxHighlighter(textDocument), enabled(true) { }
    QtSpellCheckHighlighter(QTextEdit *textEdit)
    : QSyntaxHighlighter(textEdit), enabled(true) { }

    void highlightBlock(const QString &text);
    bool enabled;
};


void QtSpellCheckHighlighter::highlightBlock(const QString &text)
{
    if (!enabled)
        return;

    QTextCharFormat spellingErrorFormat;
    spellingErrorFormat.setUnderlineColor(QColor(Qt::red));
    spellingErrorFormat.setUnderlineStyle(QTextCharFormat::SpellCheckUnderline);

    foreach(QtTextRange spellingErrorIndex, QtSpellCheckerBridge::spellingErrorIndexes(text, document())) {
        setFormat(spellingErrorIndex.index, spellingErrorIndex.length, spellingErrorFormat);
    }
}


void QtSpellCheckingTextEdit::init()
{
    ignoreSpellingAction = new QAction(tr("Ignore spelling"), this);
    learnSpellingAction = new QAction(tr("Learn spelling"), this);

    lookupSpotlightAction = new QAction(tr("Search in Spotlight"), this);
    lookupGoogleAction = new QAction(tr("Search in Google"), this);

    cutAction = new QAction(tr("Cut"), this);
    copyAction = new QAction(tr("Copy"), this);
    pasteAction = new QAction(tr("Paste"), this);
    spellingPanelAction = new QAction(tr("Spelling..."), this);
    asYouTypeAction = new QAction(tr("Check Spelling as You Type"), this);

    syntaxhighlighter = new QtSpellCheckHighlighter(this);
    asYouTypeAction->setCheckable(true);
    asYouTypeAction->setChecked(true);
}


void QtSpellCheckingTextEdit::contextMenuEvent(QContextMenuEvent * e)
{
    QTextCursor cursor = cursorForPosition(e->pos());
    cursor.movePosition(QTextCursor::StartOfWord, QTextCursor::MoveAnchor);
    cursor.movePosition(QTextCursor::EndOfWord, QTextCursor::KeepAnchor);

    currentWord = cursor.selectedText();
    const bool gotWord = (currentWord != QString());
    QMenu * const menu = new QMenu();

    // If the word is misspelt, add the suggestions to the popup menu.
    // If there are no suggestions we add a disabled "Not found action".
    if (QtSpellCheckerBridge::isSpeltCorrectly(currentWord, document()) == false) {
        const QStringList suggestions = QtSpellCheckerBridge::suggestions(currentWord);
        if (suggestions.isEmpty()) {
            QAction * const noSuggestionsAction = menu->addAction(tr("No Guesses Found"));
            noSuggestionsAction->setEnabled(false);
        } else {
            foreach (const QString &suggestion, suggestions)
                menu->addAction(suggestion);  
        }

        menu->addSeparator();
        menu->addAction(ignoreSpellingAction);
        menu->addAction(learnSpellingAction);
    }
        
    if (gotWord) {
        menu->addSeparator();
        menu->addAction(lookupSpotlightAction);
        menu->addAction(lookupGoogleAction);
    }

    menu->addSeparator();
    menu->addAction(cutAction);
    menu->addAction(copyAction);
    menu->addAction(pasteAction);
    
    cutAction->setDisabled(!gotWord);
    copyAction->setDisabled(!gotWord);
    const bool gotClipboardContents = (QApplication::clipboard()->text() == QString());
    pasteAction->setDisabled(!gotClipboardContents);
    
    menu->addSeparator();
    QMenu *spelling = new QMenu(tr("Spelling"), menu);
    menu->addMenu(spelling);
    spelling->addAction(spellingPanelAction);
    spelling->addAction(asYouTypeAction);
    
    checkedTextCursor = cursor;
    connect(menu, SIGNAL(triggered(QAction *)), SLOT(suggestedWordSelected(QAction *)));

    menu->exec(e->globalPos());
    delete menu;
 }


void QtSpellCheckingTextEdit::toggleSyntaxHighlighting()
{
    syntaxhighlighter->enabled = !syntaxhighlighter->enabled;
    syntaxhighlighter->rehighlight();
}


void QtSpellCheckingTextEdit::spotlight(const QString &word)
{
    const OSStatus resultCode = HISearchWindowShow(QtCFString::toCFStringRef(word),kNilOptions);
    if (resultCode != noErr)
        qWarning("Failed to open spotlight window");
}


void QtSpellCheckingTextEdit::openPanel(const QString &word)
{
    [[NSSpellChecker sharedSpellChecker] updateSpellingPanelWithMisspelledWord : (NSString *)QtCFString::toCFStringRef(word)];
    [[[NSSpellChecker sharedSpellChecker] spellingPanel] orderFront:nil];
}


void QtSpellCheckingTextEdit::suggestedWordSelected(QAction *action)
{
    if (action == ignoreSpellingAction) {
        QtSpellCheckerBridge::ignoreSpelling(currentWord, document());
        syntaxhighlighter->rehighlight();
    } else if (action == learnSpellingAction) {
        QtSpellCheckerBridge::learnSpelling(currentWord);    
        syntaxhighlighter->rehighlight();
    } else if (action == lookupSpotlightAction) {
        spotlight(currentWord);
    } else if (action == lookupGoogleAction) {
        const QString searchString = "http://www.google.com/search?q=" + currentWord;
        QDesktopServices::openUrl(QUrl(searchString));
    } else if (action == cutAction) {
        QApplication::clipboard()->setText(currentWord);
        checkedTextCursor.deleteChar();
    } else if (action == copyAction) {
        QApplication::clipboard()->setText(currentWord);
    } else if (action == pasteAction) {
        checkedTextCursor.insertText(action->text());
    } else if (action == spellingPanelAction) {
        openPanel(currentWord);
    } else if (action == asYouTypeAction) {
        toggleSyntaxHighlighting();
    } else {
        checkedTextCursor.insertText(action->text());
    }
}

