#ifndef ACCOUNTTYPEREGISTER_H
#define ACCOUNTTYPEREGISTER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include "qradshared.h"

namespace Ui {
    class AccountTypeRegister;
}

class AccountTypeRegister : public QDialog {
    Q_OBJECT
public:
    AccountTypeRegister(QWidget *parent = 0);
    ~AccountTypeRegister();
    void    SendAccountTypeId(int profileId);
    QString GetAccountTypeName(void);

protected:
    int     m_accountTypeId;
    QString m_accountTypeName;

    void changeEvent(QEvent *e);

private:
    Ui::AccountTypeRegister *m_ui;

private slots:
    void    SaveAccountType(void);

};

#endif // ACCOUNTTYPEREGISTER_H
