#ifndef ACCOUNTTOPAYREGISTER_H
#define ACCOUNTTOPAYREGISTER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QRegExpValidator>
#include <QSqlError>
#include "qradshared.h"
#include "accounttopaymodel.h"
#include "accounttopayhistorymodel.h"

namespace Ui {
    class AccountToPayRegister;
}

class AccountToPayRegister : public QDialog {
    Q_OBJECT
public:
    AccountToPayRegister(QWidget *parent = 0);
    ~AccountToPayRegister();
    void    SendAccountToPayId(int accountToPayId);
    QString GetAccountToPayDescription(void);

    int GetLastInsertedId();
protected:
    int                 m_accountToPayId;
    QString             m_accountToPayDescription;
    QRegExpValidator    *m_expMoney;
    QSqlQueryModel      *m_modelAccountType,
                        *m_modelSupplier,
                        *m_modelBank;

    void    changeEvent(QEvent *e);
    void    GetAccountTypeValues(void);
    void    GetSupplierValues(void);
    void    GetBankValues(void);

private:
    Ui::AccountToPayRegister *m_ui;

private slots:
    void    SaveAccountToPay(void);
    void    changeFocus(void);

};

#endif // ACCOUNTTOPAYREGISTER_H
