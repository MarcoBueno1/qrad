#ifndef MANAGERDweller_H
#define MANAGERDweller_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editdweller.h"

namespace Ui {
class ManagerDweller;
}

class ManagerDweller : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerDweller(QWidget *parent = 0);
    ~ManagerDweller();

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
    Ui::ManagerDweller *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(Dweller *newest );

};

#endif // MANAGERDweller_H
