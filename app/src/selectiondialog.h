#ifndef SELECTIONDIALOG_H
#define SELECTIONDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
class SelectionDialog;
}

class NumberEdit;

class SelectionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SelectionDialog(QWidget *parent = nullptr);
    ~SelectionDialog();

    void setValue(qulonglong value);
    qulonglong value() const;

    void clear();

signals:
    void valueChanged(qulonglong);

private:
    void onValueChange(qulonglong value);
    void clearIfEmpty(const QString& text);

    Ui::SelectionDialog *ui;
    QList<NumberEdit*> mEditors;
};

#endif // SELECTIONDIALOG_H
