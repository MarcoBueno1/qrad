#ifndef BANKREGISTER_H
#define BANKREGISTER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QSqlError>
#include "dsm.h"
#include "bankmodel.h"

namespace Ui {
    class BankRegister;
}

class BankRegister : public QDialog {
    Q_OBJECT
public:
    BankRegister(QWidget *parent = 0);
    ~BankRegister();
    void    SendBankId(int profileId);
    QString GetBankName(void);

protected:
    int     m_bankId;
    QString m_bankName;

    void changeEvent(QEvent *e);

private:
    Ui::BankRegister *m_ui;

private slots:
    void    SaveBank(void);

};

#endif // BANKREGISTER_H
