#ifndef ACCOUNTTOPAYMANAGER_H
#define ACCOUNTTOPAYMANAGER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QKeyEvent>
#include "dsm.h"
#include "accounttopayregister.h"
#include "paidaccount.h"
#include "dsmreport.h"

namespace Ui {
    class AccountToPayManager;
}

class AccountToPayManager : public QDialog {
    Q_OBJECT
public:
    AccountToPayManager(QWidget *parent = 0);
    ~AccountToPayManager();

protected:
    QSqlQueryModel  *m_modelAccountType,
                    *m_modelSupplier,
                    *m_modelBank,
                    *m_selectAccountToPay;
    int             m_accountToPayId,
                    m_accountToPayRow;
    QString         m_dateStr,
                    m_strAux;

    bool            m_accountToPayPaid;

    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *event);
    void    resizeEvent(QResizeEvent *event);
    void    closeEvent(QCloseEvent *event);
    void    ConfigHeaderTable(void);
    void    InitialConfig(int row = 0);
    void    keyPressEvent(QKeyEvent *event);
    void    GetAccountTypeValues(void);
    void    GetSupplierValues(void);
    void    GetBankValues(void);

private:
    Ui::AccountToPayManager *m_ui;

private slots:
    void    fillTheFields(QModelIndex);
    void    NewAccountToPay(void);
    void    EditAccountToPay(void);
    void    PayAccount(void);
    void    DeleteAccountToPay(void);
    void    GetAccountToPay(void);
    void    ShowReport(void);
};

#endif // ACCOUNTTOPAYMANAGER_H
