#ifndef ACCOUNTCALENDAR_H
#define ACCOUNTCALENDAR_H

#include <QDialog>
#include "ui_accountcalendar.h"
#include "accountcalendaraux.h"

namespace Ui{
   class AccountCalendar;
};

class AccountCalendar : public QDialog {
    Q_OBJECT
public:
    AccountCalendar(QWidget *parent = 0);
    ~AccountCalendar();

protected:
    QSqlQueryModel                  *m_accountModel;
    QList<DSM_AccountCalendar_t>    m_accountCalendar;
    QDate                           m_date;

    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *e);
    void    closeEvent(QCloseEvent *event);
    void    keyPressEvent(QKeyEvent *event);

private:
    Ui::AccountCalendar* m_ui;

private slots:
    void    UpdateCalendar(int, int);
    void    UpdateDate(void);
    void    ShowDetail(QDate);
};

#endif // ACCOUNTCALENDAR_H
