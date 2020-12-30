#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "basedialog.h"

class SelectionDialog;

class MainWindow : public BaseDialog
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

protected:
    void moveEvent(QMoveEvent* e) override;

private:
    void onSelectionChange();
    void moveSelectionDialog();
    void changeSelectedText(qulonglong value);

    SelectionDialog* mSelectionDialog = nullptr;
    QList<NumberEdit*> mSelectableEditors;
};
#endif // MAINWINDOW_H
