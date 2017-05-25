#ifndef ACCOUNTTYPEREPORT_H
#define ACCOUNTTYPEREPORT_H

#include <QDialog>
#include "ui_accounttypereport.h"
#include "dsmreport.h"

namespace Ui{
   class AccountTypeReport;
};

class AccountTypeReport : public QDialog {
    Q_OBJECT
public:
    AccountTypeReport(QWidget *parent = 0);
    ~AccountTypeReport();

protected:
    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *e);
    void    keyPressEvent(QKeyEvent *e);

private:
    Ui::AccountTypeReport* m_ui;

private slots:
    void    Launcher(void);
};

#endif // ACCOUNTTYPEREPORT_H
