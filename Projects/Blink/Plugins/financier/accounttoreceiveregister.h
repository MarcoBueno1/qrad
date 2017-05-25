#ifndef ACCOUNTTORECEIVEREGISTER_H
#define ACCOUNTTORECEIVEREGISTER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QRegExpValidator>
#include <QSqlError>
#include "qradshared.h"
#include "accounttoreceivemodel.h"
#include "accounttoreceivehistorymodel.h"

namespace Ui {
    class AccountToReceiveRegister;
}

class AccountToReceiveRegister : public QDialog {
    Q_OBJECT
public:
    AccountToReceiveRegister(QWidget *parent = 0);
    ~AccountToReceiveRegister();
    void    SendAccountToReceiveId(int accountToReceiveId);
    QString GetAccountToReceiveDescription(void);

protected:
    int                 m_accountToReceiveId;
    QString             m_accountToReceiveDescription;
    QRegExpValidator    *m_expMoney;
    QSqlQueryModel      *m_modelAccountType,
                        *m_modelClient;

    void    changeEvent(QEvent *e);
    void    GetAccountTypeValues(void);
    void    GetClientValues(void);

private:
    Ui::AccountToReceiveRegister *m_ui;

private slots:
    void    SaveAccountToReceive(void);
    void    changeFocus(void);

};

#endif // ACCOUNTTORECEIVEREGISTER_H
