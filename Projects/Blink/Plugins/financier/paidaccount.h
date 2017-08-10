#ifndef PAIDACCOUNT_H
#define PAIDACCOUNT_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
//#include "dsm.h"
#include "accounttopaymodel.h"
#include "accounttoreceivemodel.h"

#include <QRegExpValidator>

namespace Ui {
    class PaidAccount;
}

class PaidAccount : public QDialog {
    Q_OBJECT
public:
    PaidAccount(QWidget *parent = 0);
    ~PaidAccount();
    void    SendPaidAccountId(int accountId, DSM_AccountType_t accountType);

    double getValuePaid();
protected:
    int                 m_accountId;
    DSM_AccountType_t   m_accountType;
    QRegExpValidator    *m_expMoney;

    void changeEvent(QEvent *e);

private:
    Ui::PaidAccount *m_ui;

private slots:
    void    SavePaidAccount(void);

};

#endif // PAIDACCOUNT_H
