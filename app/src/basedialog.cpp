#include "basedialog.h"
#include "ui_basedialog.h"

BaseDialog::BaseDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BaseDialog)
{
    ui->setupUi(this);

    ui->hex->setBase(16);
    ui->hex->setGroupSize(4);

    ui->dec->setBase(10);
    ui->dec->setGroupSize(3);

    ui->bin->setBase(2);
    ui->bin->setGroupSize(4);

    mEditors = { ui->hex, ui->dec, ui->bin };

    for (NumberEdit* editor: mEditors)
    {
        connect(editor, &NumberEdit::valueChanged, this, &BaseDialog::onValueChange);
        connect(editor, &NumberEdit::textChanged, this, &BaseDialog::clearIfEmpty);
    }
}

BaseDialog::~BaseDialog()
{
    delete ui;
}

void BaseDialog::setValue(qulonglong value)
{
    for (NumberEdit* e: mEditors)
    {
        QSignalBlocker lock(e);
        e->setValue(value);
    }
}

qulonglong BaseDialog::value() const
{
    return ui->hex->value();
}

void BaseDialog::clear()
{
    for (NumberEdit* e: mEditors) e->clear();
}

void BaseDialog::onValueChange(qulonglong value)
{
    NumberEdit* editor = qobject_cast<NumberEdit*>(sender());
    Q_ASSERT(editor); if (!editor) return;

    for (NumberEdit* target : mEditors)
    {
        if (target != editor)
        {
            QSignalBlocker lock(target);
            target->setValue(value);
        }
    }

    emit valueChanged(value);
}

void BaseDialog::clearIfEmpty(const QString& text)
{
    if (!text.isEmpty()) return;

    NumberEdit* editor = qobject_cast<NumberEdit*>(sender());
    Q_ASSERT(editor); if (!editor) return;

    for (NumberEdit* target : mEditors)
    {
        if (target != editor)
        {
            QSignalBlocker lock(target);
            target->clear();
        }
    }
}
