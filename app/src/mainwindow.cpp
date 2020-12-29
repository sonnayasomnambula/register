#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
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
}

MainWindow::~MainWindow()
{
    delete ui;
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
        return;
    }

    qDebug() << selection;
    ui->statusbar->showMessage(QString::asprintf(qPrintable(tr("Selection: 0x%02llX (%llu) [bits %d..%d]")),
                                                 selection, selection, from, to));
}

