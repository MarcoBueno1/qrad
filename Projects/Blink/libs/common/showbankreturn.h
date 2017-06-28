#ifndef SHOWBANKRETURN_H
#define SHOWBANKRETURN_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "bankticketparserfactory.h"

namespace Ui {
class ShowBankReturn;
}

class ShowBankReturn : public QDialog
{
    Q_OBJECT

public:
    explicit ShowBankReturn(QWidget *parent = 0);
    ~ShowBankReturn();

    bool Exec(QList<BankTicket*> *list, QString Path);

private:
    Ui::ShowBankReturn *ui;
    QSqlDatabase ConfigTempDatabase();
    void BuildTable(QList<BankTicket *> *list);
    QSqlQueryModel *m_model;
    QSqlDatabase db;
};

#endif // SHOWBANKRETURN_H
