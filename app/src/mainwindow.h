#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class SelectionDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void moveEvent(QMoveEvent* e) override;

private:
    void onValueChange(qulonglong value);
    void clearIfEmpty(const QString& text);
    void onSelectionChange(qulonglong selection, int from, int to);
    void moveSelectionDialog();
    void changeSelectedText(qulonglong value);

    Ui::MainWindow* ui;
    SelectionDialog* mSelectionDialog = nullptr;
};
#endif // MAINWINDOW_H
