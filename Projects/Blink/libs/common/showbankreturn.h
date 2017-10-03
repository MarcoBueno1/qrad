#ifndef SHOWBANKRETURN_H
#define SHOWBANKRETURN_H

#include <QDialog>
#include <QSqlDatabase>
#include <QSqlQueryModel>

#include "bankticketparserfactory.h"
#include "column2delegate.h"

namespace Ui {
class ShowBankReturn;
}

class ShowBankReturn : public QDialog
{
    Q_OBJECT

public:
    explicit ShowBankReturn(QWidget *parent = 0);
    ~ShowBankReturn();

    bool Exec(QList<BankTicket*> *list, QStringList Paths);
    bool Exec(QList<BankTicket*> *list, QString Path);

public slots:
    void RadioBtnChanged();
private:
    Ui::ShowBankReturn *ui;
    QSqlDatabase ConfigTempDatabase();
    void BuildTable(QList<BankTicket *> *list);
    QSqlQueryModel *m_model;
    QSqlDatabase db;
    QString FriendlyOpType(QRAD_BANKTKT_TYPEOP opType);
    ColumnRetBankAndPaid *m_ColumnPaid;
};

#endif // SHOWBANKRETURN_H
