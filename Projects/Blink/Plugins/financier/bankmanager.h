#ifndef BANKMANAGER_H
#define BANKMANAGER_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QKeyEvent>
#include "dsm.h"
#include "bankregister.h"

namespace Ui {
    class BankManager;
}

class BankManager : public QDialog {
    Q_OBJECT
public:
    BankManager(QWidget *parent = 0);
    ~BankManager();

protected:
    QSqlQueryModel  *m_selectBank;
    int             m_bankId;
    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent *event);
    void    ConfigHeaderTable(void);
    void    InitialConfig(QString banckName = "");
    void    keyPressEvent(QKeyEvent *event);
    void    GetBank(void);
    void    ShowCurrentInformations(void);

private:
    Ui::BankManager *m_ui;

private slots:
    void    fillTheFields(QModelIndex);
    void    SearchBank(QString);
    void    NewBank(void);
    void    EditBank(void);
    void    DeleteBank(void);
    void    TableClicked(QModelIndex);

};

#endif // BANKMANAGER_H
