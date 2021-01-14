#include "numberedit.h"

#include <QEvent>
#include <QFontDatabase>
#include <QKeyEvent>

NumberEdit::NumberEdit(QWidget *parent) :
    NumberEdit(10, 3, parent)
{

}

NumberEdit::NumberEdit(int base, int groupSize, QWidget* parent) :
    QLineEdit(parent),
    mBase(base),
    mGroupSize(groupSize),
    mValidator(this)
{
    setValidator(&mValidator);
    connect(this, &NumberEdit::textChanged, this, &NumberEdit::onTextChange);

    QFont monospace("Monospace");
#ifdef Q_OS_WIN
    monospace.setStyleHint(QFont::TypeWriter);
#else
    monospace.setStyleHint(QFont::Monospace);
#endif
    monospace.setPointSize(24);
    setFont(monospace);
}

void NumberEdit::setValue(qulonglong value)
{
    QSignalBlocker lock(this);
    setText(separated(QString::number(value, mBase), mGroupSize));
    lock.unblock();

    emit textChanged(text());
}

qulonglong NumberEdit::value() const
{
    QString s = text();
    s.remove(mcSeparator);
    return s.toULongLong(nullptr, mBase);
}

void NumberEdit::setGroupSize(int value)
{
    ValueBlocker lock(this, true);
    mGroupSize = value;
}

void NumberEdit::setBase(int value)
{
    ValueBlocker lock(this, true);
    mBase = value;
}

QString NumberEdit::separated(const QString number, int groupSize)
{
    QString s = number;
    s.remove(mcSeparator);
    QString n;
    while (!s.isEmpty())
    {
        if (!n.isEmpty())
            n.prepend(mcSeparator);
        n.prepend(s.right(groupSize));
        s.chop(groupSize);
    }

    return n;
}

std::tuple<qulonglong, int, int> NumberEdit::selectedBits() const
{
    return extractSelection(text(), selectionStart(), selectionEnd(), mBase);
}

void NumberEdit::onTextChange(const QString& text)
{
    int p = cursorPosition();
    QString s = separated(text, mGroupSize);
    setText(s);
    p += s.size() - text.size();
    setCursorPosition(p);

    emit valueChanged(value());
}

///
/// \brief NumberEdit::extractSelection
/// \param text     Entire text
/// \param start    Selection left edge (0 = before first character)
/// \param end      Selection right edge
/// \param base     Base, 16 for hex and 2 for bin
/// \return a number in selected text
///         first selected bit
///         last selected bit
///
std::tuple<qulonglong, int, int> NumberEdit::extractSelection(const QString& text, int start, int end, int base)
{
    if (start == end)
    {
        // nothing selected
        return {0, 0, 0};
    }

    QString selectedText = text.mid(start, end - start);
    selectedText.remove(mcSeparator);
    qulonglong selection = selectedText.toULongLong(nullptr, base);

    int len = text.length();
    int from = start;
    int to = end;

    for (int i = 0; i < start; ++i)
    {
        if (text[i] == mcSeparator)
        {
            --from;
            --to;
            --len;
        }
    }

    for (int i = start; i < end; ++i)
    {
        if (text[i] == mcSeparator)
        {
            --to;
            --len;
        }
    }

    for (int i = end; i < text.length(); ++i)
    {
        if (text[i] == mcSeparator)
        {
            --len;
        }
    }

    int factor = base == mcBase16 ? 4 : 1;

    from = len - from;
    from = from * factor - 1;

    to = (len - to);
    to *= (factor);

    return {selection, from, to};
}

void NumberEdit::changeSelectedText(qulonglong value)
{
    QString original = selectedText();
    int spaces = original.count(mcSeparator);

    QString number = QString::number(value, mBase);
    const bool wrong = original.size() < number.size() + spaces;
    if (wrong) return;

    number.prepend(QString(original.size() - number.size() - spaces, '0'));
    for (int i = -1; i = original.indexOf(mcSeparator, i + 1), i != -1;)
        number.insert(i, mcSeparator);

    int sst = selectionStart();
    int ssl = selectionLength();
    QString entire = text();
    entire.remove(sst, ssl);
    entire.insert(sst, number);
    setText(entire);
    setSelection(sst, ssl);
}

QValidator::State NumberEdit::Validator::validate(QString& input, int&) const
{
    if (input.isEmpty())
        return QValidator::Acceptable;

    input = input.toUpper();

    bool ok = false;
    QString s = input;
    s.remove(mcSeparator);
    s.toULongLong(&ok, mParent->base());
    return ok ? QValidator::Acceptable : QValidator::Invalid;
}
