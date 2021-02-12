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
    void moveSelectionDialog(bool force = false);
    void changeSelectedText(qulonglong value);
    void switchFocus();

    bool eventFilter(QObject* o, QEvent* e) override;

    SelectionDialog* mSelectionDialog = nullptr;
    const QList<NumberEdit*> mSelectableEditors;
    bool mSticking = true;
};
#endif // MAINWINDOW_H
