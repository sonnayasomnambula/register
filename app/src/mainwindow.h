#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void onValueChange(qulonglong value);
    void clearIfEmpty(const QString& text);
    void onSelectionChange(qulonglong selection, int from, int to);

    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
