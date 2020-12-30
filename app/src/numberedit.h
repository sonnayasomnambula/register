#ifndef NUMBEREDIT_H
#define NUMBEREDIT_H

#include <QLineEdit>
#include <QValidator>

template<class T>
class ValueBlocker
{
public:
    explicit ValueBlocker(T* widget, bool blockSignals = false) :
        mWidget(widget),
        mValue(mWidget->value()),
        mLock(blockSignals ? mWidget : nullptr),
        mIsEmpty(mWidget->text().isEmpty())
    {

    }

    ~ValueBlocker()
    {
        if (!mWidget)
            return;

        if (mIsEmpty)
            mWidget->clear();
        else
            mWidget->setValue(mValue);
    }

private:
    T* mWidget = nullptr;
    decltype(mWidget->value()) mValue;
    QSignalBlocker mLock;
    bool mIsEmpty = false;
};

class NumberEdit : public QLineEdit
{
    Q_OBJECT

public:
    explicit NumberEdit(QWidget* parent = nullptr);
    NumberEdit(int base, int groupSize, QWidget* parent = nullptr);

    void setValue(qulonglong value);
    qulonglong value() const;

    void setBase(int value);
    int base() const { return mBase; }

    void setGroupSize(int value);
    int groupSize() const { return mGroupSize; }

    static QString separated(const QString number, int groupSize);
    static std::tuple<qulonglong, int, int> extractSelection(const QString& text, int start, int end, int base);
    void changeSelectedText(qulonglong value);

    static const int mcBase2 = 2;
    static const int mcBase16 = 16;

signals:
    void valueChanged(qulonglong);
    void selectionChanged2(qulonglong selection, int from, int to); // TODO rename

private:
    void onTextChange(const QString& text);
    void onSelectionChange();

    int mBase = 10;
    int mGroupSize = 3;

    static constexpr QChar mcSeparator = ' ';

    class Validator: public QValidator
    {
    public:
        explicit Validator(const NumberEdit* parent) : mParent(parent) {}
        QValidator::State validate(QString& input, int& pos) const override;

    private:
        const NumberEdit* mParent;
    } mValidator;
};

#endif // NUMBEREDIT_H
