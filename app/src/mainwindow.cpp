#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QTimer>

#include "selectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSelectionDialog(new SelectionDialog(this))
{
    ui->setupUi(this);

    ui->hex->setBase(16);
    ui->hex->setGroupSize(4);

    ui->dec->setBase(10);
    ui->dec->setGroupSize(3);

    ui->bin->setBase(2);
    ui->bin->setGroupSize(4);

    connect(ui->hex, &NumberEdit::valueChanged, this, &MainWindow::onValueChange);
    connect(ui->dec, &NumberEdit::valueChanged, this, &MainWindow::onValueChange);
    connect(ui->bin, &NumberEdit::valueChanged, this, &MainWindow::onValueChange);

    connect(ui->hex, &NumberEdit::textChanged, this, &MainWindow::clearIfEmpty);
    connect(ui->dec, &NumberEdit::textChanged, this, &MainWindow::clearIfEmpty);
    connect(ui->bin, &NumberEdit::textChanged, this, &MainWindow::clearIfEmpty);

    connect(ui->hex, &NumberEdit::selectionChanged2, this, &MainWindow::onSelectionChange);
    connect(ui->bin, &NumberEdit::selectionChanged2, this, &MainWindow::onSelectionChange);

    mSelectionDialog->show();
    QTimer::singleShot(50, this, &MainWindow::moveSelectionDialog);

    connect(mSelectionDialog, &SelectionDialog::valueChanged, this, &MainWindow::changeSelectedText);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::moveEvent(QMoveEvent*)
{
    moveSelectionDialog();
}

void MainWindow::onValueChange(qulonglong value)
{
    NumberEdit* editor = qobject_cast<NumberEdit*>(sender());
    Q_ASSERT(editor); if (!editor) return;

    auto set = [](NumberEdit* sender, NumberEdit* target, qulonglong value){
        if (sender != target)
        {
            QSignalBlocker lock(target);
            target->setValue(value);
        }
    };

    set(editor, ui->hex, value);
    set(editor, ui->dec, value);
    set(editor, ui->bin, value);
}

void MainWindow::clearIfEmpty(const QString& text)
{
    NumberEdit* editor = qobject_cast<NumberEdit*>(sender());
    Q_ASSERT(editor); if (!editor) return;

    auto clr = [](NumberEdit* sender, NumberEdit* target, const QString& text){
        if (sender != target && text.isEmpty())
        {
            QSignalBlocker lock(target);
            target->clear();
        }
    };

    clr(editor, ui->hex, text);
    clr(editor, ui->dec, text);
    clr(editor, ui->bin, text);
}

void MainWindow::onSelectionChange(qulonglong selection, int from, int to)
{
    if (from == 0 && to == 0 && selection == 0)
    {
        ui->statusbar->clearMessage();
        mSelectionDialog->clear();
        return;
    }

    ui->statusbar->showMessage(QString::asprintf(qPrintable(tr("Bits %d..%d selected")), from, to));
    mSelectionDialog->setValue(selection);
}

void MainWindow::moveSelectionDialog()
{
    QPoint topRight = mapToGlobal(frameGeometry().topRight());
#ifdef Q_OS_LINUX
    // works in XUbuntu / XFCE 4.14
    QPoint topLeft = mapToGlobal(frameGeometry().topLeft());
    const int factor = 2;
    topLeft.rx() /= factor;
    topLeft.ry() /= factor;
    topRight -= topLeft;
#endif
    topRight.ry() -= 12;
    mSelectionDialog->move(topRight);
}

void MainWindow::changeSelectedText(qulonglong value)
{
    QList<NumberEdit*> editors = { ui->hex, ui->bin };
    for (NumberEdit* edit: editors)
    {
        if (edit->hasSelectedText())
            edit->changeSelectedText(value);
    }
}

