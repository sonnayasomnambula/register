#include "mainwindow.h"
#include "ui_basedialog.h"

#include <QDebug>
#include <QTimer>

#include "selectiondialog.h"

MainWindow::MainWindow(QWidget *parent)
    : BaseDialog(parent, Qt::Window)
    , mSelectionDialog(new SelectionDialog(this))
{
    setWindowTitle(tr("Register"));

    mSelectableEditors = { ui->hex, ui->bin };
    for (auto edit: mSelectableEditors)
        connect(edit, &NumberEdit::selectionChanged, this, &MainWindow::onSelectionChange);

    mSelectionDialog->resize(mSelectionDialog->width() / 3, mSelectionDialog->height());
    mSelectionDialog->show();

    connect(mSelectionDialog, &SelectionDialog::valueChanged, this, &MainWindow::changeSelectedText);

    QTimer::singleShot(100, [this]{ moveSelectionDialog(true); });
}

void MainWindow::moveEvent(QMoveEvent*)
{
    moveSelectionDialog();
}

void MainWindow::onSelectionChange()
{
    auto edit = qobject_cast<NumberEdit*>(sender());
    Q_ASSERT(edit); if (!edit) return;

    if (!edit->hasSelectedText())
    {
        ui->statusbar->clearMessage();
        mSelectionDialog->clear();
        return;
    }

    auto [selection, from, to] = edit->selectedBits();
    ui->statusbar->showMessage(QString::asprintf(qPrintable(tr("Bits %d..%d selected")), from, to));
    mSelectionDialog->setValue(selection);
}

void MainWindow::moveSelectionDialog(bool force)
{
    QPoint topRight = mapToGlobal(frameGeometry().topRight());
#ifdef Q_OS_LINUX
    {
        // works in XUbuntu / XFCE 4.14
        QPoint topLeft = mapToGlobal(frameGeometry().topLeft());
        const int factor = 2;
        topLeft.rx() /= factor;
        topLeft.ry() /= factor;
        topRight -= topLeft;
    }
#endif
    topRight.ry() -= 13;

    QPoint topLeft = mSelectionDialog->mapToGlobal(mSelectionDialog->frameGeometry().topLeft());
#ifdef Q_OS_LINUX
    // WTF???
    const int factor = 2;
    topLeft.rx() /= factor;
    topLeft.ry() /= factor;
#endif
    QPoint diff = topLeft - topRight;
    const int maxDistance = 120;
    bool near = std::abs(diff.x()) < maxDistance && std::abs(diff.y()) < maxDistance;
    if (near || force)
        mSelectionDialog->move(topRight);
}

void MainWindow::changeSelectedText(qulonglong value)
{
    for (NumberEdit* edit: mSelectableEditors)
        if (edit->hasSelectedText())
            edit->changeSelectedText(value);
}

