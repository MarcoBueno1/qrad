#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void resizeEvent(QResizeEvent *event);
private slots:
    void on_actionSobre_o_Blink_triggered();

    void on_actionBloquear_triggered();

private:
    Ui::MainWindow *ui;
    void setRemainingDays(int value);
    void AdjustMenuPermissions();
    void enumerateMenu(QMenu *menu);
};

#endif // MAINWINDOW_H
