#ifndef MANAGERbankaccount_H
#define MANAGERbankaccount_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editbankaccount.h"

namespace Ui {
class Managerbankaccount;
}

class Managerbankaccount : public QDialog
{
    Q_OBJECT

public:
    explicit Managerbankaccount(QWidget *parent = 0);
    ~Managerbankaccount();

public slots:
    void notFound();
    void Found( QModelIndex );
    void StartTimer( QString );
    void TableClicked( QModelIndex );
    void CurrentChanged( QModelIndex );
    void KeyPressTimeout();
    void doEditar();
    void doSair();
    void doNovo();

private:
    Ui::Managerbankaccount *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(bankaccount *newest );

};

#endif // MANAGERbankaccount_H
