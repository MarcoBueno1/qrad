#ifndef MANAGERreserve_H
#define MANAGERreserve_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editreserve.h"

namespace Ui {
class Managerreserve;
}

class Managerreserve : public QDialog
{
    Q_OBJECT

public:
    explicit Managerreserve(QWidget *parent = 0);
    ~Managerreserve();

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
    Ui::Managerreserve *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(reserve *newest );

};

#endif // MANAGERreserve_H
