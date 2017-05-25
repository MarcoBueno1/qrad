#ifndef ACCOUNTTORECEIVEMANAGER_H
#define ACCOUNTTORECEIVEMANAGER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QKeyEvent>
#include "dsm.h"
#include "accounttoreceiveregister.h"
#include "paidaccount.h"
#include "dsmreport.h"

namespace Ui {
    class AccountToReceiveManager;
}

class AccountToReceiveManager : public QDialog {
    Q_OBJECT
public:
    AccountToReceiveManager(QWidget *parent = 0);
    ~AccountToReceiveManager();

protected:
    QSqlQueryModel  *m_modelAccountType,
                    *m_modelClient,
                    *m_modelBank,
                    *m_selectAccountToReceive;
    int             m_accountToReceiveId,
                    m_accountToReceiveRow;
    QString         m_dateStr,
                    m_strAux;

    bool            m_accountToReceivePaid;

    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *event);
    void    resizeEvent(QResizeEvent *event);
    void    closeEvent(QCloseEvent *event);
    void    ConfigHeaderTable(void);
    void    InitialConfig(int row = 0);
    void    keyPressEvent(QKeyEvent *event);
    void    GetAccountTypeValues(void);
    void    GetClientValues(void);

private:
    Ui::AccountToReceiveManager *m_ui;

private slots:
    void    fillTheFields(QModelIndex);
    void    NewAccountToReceive(void);
    void    EditAccountToReceive(void);
    void    PayAccount(void);
    void    DeleteAccountToReceive(void);
    void    GetAccountToReceive(void);
    void    ShowReport(void);
};

#endif // ACCOUNTTORECEIVEMANAGER_H
