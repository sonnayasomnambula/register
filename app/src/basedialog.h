#ifndef BASEDIALOG_H
#define BASEDIALOG_H

#include <QDialog>
#include <QList>

namespace Ui {
class BaseDialog;
}

class NumberEdit;

class BaseDialog : public QDialog
{
    Q_OBJECT

public:
    explicit BaseDialog(QWidget *parent = nullptr, Qt::WindowFlags flags = Qt::WindowFlags());
    ~BaseDialog() override;

    void setValue(qulonglong value);
    qulonglong value() const;

    void clear();

signals:
    void valueChanged(qulonglong);

protected:
    Ui::BaseDialog* ui;
    QList<NumberEdit*> mEditors;

private:
    void onValueChange(qulonglong value);
    void clearIfEmpty(const QString& text);
};

#endif // BASEDIALOG_H
