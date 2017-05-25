#ifndef ACCOUNTHISTORYREPORT_H
#define ACCOUNTHISTORYREPORT_H

#include <QDialog>
#include "ui_accounthistoryreport.h"
#include "accounthistoryreportaux.h"

namespace Ui{
   class AccountHistoryReport;
};

class AccountHistoryReport : public QDialog {
    Q_OBJECT
public:
    AccountHistoryReport(QWidget *parent = 0);
    ~AccountHistoryReport();

protected:
    QSqlQueryModel  *m_accountModel,
                    *m_userModel;

    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *e);
    void    keyPressEvent(QKeyEvent *e);
    void    ConfigHeaderTable(void);
    void    GetUserValues(void);

private:
    Ui::AccountHistoryReport* m_ui;

private slots:
    void    Launcher(void);
    void    InitialConfig(int row = 0);
};

#endif // ACCOUNTHISTORYREPORT_H
