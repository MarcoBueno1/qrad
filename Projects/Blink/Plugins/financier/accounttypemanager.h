#ifndef ACCOUNTTYPEMANAGER_H
#define ACCOUNTTYPEMANAGER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QKeyEvent>
#include "qradshared.h"
#include "accounttyperegister.h"

namespace Ui {
    class AccountTypeManager;
}

class AccountTypeManager : public QDialog {
    Q_OBJECT
public:
    AccountTypeManager(QWidget *parent = 0);
    ~AccountTypeManager();

protected:
    QSqlQueryModel  *m_selectAccountType;
    int             m_accountTypeId;
    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *event);
    void    ConfigHeaderTable(void);
    void    InitialConfig(QString groupName = "");
    void    keyPressEvent(QKeyEvent *event);
    void    GetAccountType(void);
    void    ShowCurrentInformations(void);

private:
    Ui::AccountTypeManager *m_ui;

private slots:
    void    fillTheFields(QModelIndex);
    void    SearchAccountType(QString);
    void    NewAccountType(void);
    void    EditAccountType(void);
    void    DeleteAccountType(void);
    void    TableClicked(QModelIndex);

};

#endif // ACCOUNTTYPEMANAGER_H
