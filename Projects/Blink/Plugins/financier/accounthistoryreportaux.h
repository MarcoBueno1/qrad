#ifndef ACCOUNTHISTORYREPORTAUX_H
#define ACCOUNTHISTORYREPORTAUX_H

#include <QDialog>
#include "ui_accounthistoryreportaux.h"

namespace Ui{
   class AccountHistoryReportAux;
};

class AccountHistoryReportAux : public QDialog {
    Q_OBJECT
public:
    AccountHistoryReportAux(QWidget *parent = 0);
    ~AccountHistoryReportAux();

    void    SendAccountId(int accountId, DSM_AccountType_t accountType);

protected:
    QSqlQueryModel      *m_accountModel;
    int                 m_accountId;
    DSM_AccountType_t   m_accountType;

    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *e);
    void    ConfigHeaderTable(void);

private:
    Ui::AccountHistoryReportAux* m_ui;

private slots:
    void    InitialConfig();
};

#endif // ACCOUNTHISTORYREPORTAUX_H
