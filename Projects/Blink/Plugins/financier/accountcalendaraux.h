#ifndef ACCOUNTHISTORYREPORTAUX_H
#define ACCOUNTHISTORYREPORTAUX_H

#include <QDialog>
#include "ui_accountcalendaraux.h"
#include "dsmreport.h"
#include "column2delegate.h"

namespace Ui{
   class AccountCalendarAux;
};

class AccountCalendarAux : public QDialog {
    Q_OBJECT
public:
    AccountCalendarAux(QWidget *parent = 0);
    ~AccountCalendarAux();

    void    SendDate(QDate date);

protected:
    QSqlQueryModel      *m_accountToPayModel,
                        *m_accountToReceiveModel;
    QDate               m_date;

    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *e);
    void    ConfigHeaderTable(void);

private:
    Ui::AccountCalendarAux* m_ui;

private slots:
    void    InitialConfig();
};

#endif // ACCOUNTHISTORYREPORTAUX_H
