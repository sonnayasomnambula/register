#include "selectiondialog.h"
#include "ui_basedialog.h"

SelectionDialog::SelectionDialog(QWidget *parent) :
    BaseDialog(parent)
{
    setWindowTitle(tr("Selection"));

    ui->labelHex->hide();
    ui->labelDec->hide();
    ui->labelBin->hide();
}
