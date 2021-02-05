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

    mSelectionDialog->installEventFilter(this);
    mSelectionDialog->resize(mSelectionDialog->width() / 3, mSelectionDialog->height());
    mSelectionDialog->show();

    connect(mSelectionDialog, &SelectionDialog::valueChanged, this, &MainWindow::changeSelectedText);

    QTimer::singleShot(100, this, [this]{ moveSelectionDialog(true); });
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
    ui->statusbar->showMessage(tr("Bits %1..%2 selected").arg(from).arg(to));
    mSelectionDialog->setValue(selection);
}

void MainWindow::moveSelectionDialog(bool force)
{
    mSticking |= force;
    if (mSticking)
        mSelectionDialog->move(frameGeometry().topRight());
}

void MainWindow::changeSelectedText(qulonglong value)
{
    for (NumberEdit* edit: mSelectableEditors)
        if (edit->hasSelectedText())
            edit->changeSelectedText(value);
}

bool MainWindow::eventFilter(QObject* o, QEvent* e)
{
    if (o == mSelectionDialog && e->type() == QEvent::Move)
    {
        QPoint distance = mSelectionDialog->pos() - frameGeometry().topRight();
        const int captureDistance = 50;
        bool near = std::abs(distance.x()) < captureDistance && std::abs(distance.y()) < captureDistance;
        if (mSticking != near)
        {
            mSticking = near;
            if (mSticking)
            {
                QTimer::singleShot(0, this, [this]{ moveSelectionDialog(); });
            }
        }
    }

    return QObject::eventFilter(o, e);
}
