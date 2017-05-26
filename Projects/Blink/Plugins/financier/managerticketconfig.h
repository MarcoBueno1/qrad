#ifndef MANAGERticketconfig_H
#define MANAGERticketconfig_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editticketconfig.h"

namespace Ui {
class Managerticketconfig;
}

class Managerticketconfig : public QDialog
{
    Q_OBJECT

public:
    explicit Managerticketconfig(QWidget *parent = 0);
    ~Managerticketconfig();

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
    Ui::Managerticketconfig *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(ticketconfig *newest );

};

#endif // MANAGERticketconfig_H
