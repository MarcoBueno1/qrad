#ifndef SHOWPAYMENTS_H
#define SHOWPAYMENTS_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class ShowPayments;
}

class ShowPayments : public QDialog
{
    Q_OBJECT

public:
    explicit ShowPayments(QWidget *parent = 0);
    ~ShowPayments();

    void Exec(QString Path);
private:
    Ui::ShowPayments *ui;
    QSqlQueryModel *m_model;
    void PersistPayments();
};

#endif // SHOWPAYMENTS_H
