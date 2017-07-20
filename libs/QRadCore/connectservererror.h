#ifndef CONNECTSERVERERROR_H
#define CONNECTSERVERERROR_H

#include <QDialog>

namespace Ui {
class ConnectServerError;
}

class ConnectServerError : public QDialog
{
    Q_OBJECT
    
public:
    explicit ConnectServerError(QWidget *parent = 0);
    ~ConnectServerError();
    void getError(QString text);

protected:
    void showEvent(QShowEvent *e);
    
private slots:
    void on_btnExit_clicked();

    void on_btnSave_clicked();

private:
    Ui::ConnectServerError *ui;
};

#endif // CONNECTSERVERERROR_H
