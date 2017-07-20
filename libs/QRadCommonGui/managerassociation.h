#ifndef MANAGERassociation_H
#define MANAGERassociation_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
//#include "editassociation.h"

namespace Ui {
class Managerassociation;
}

class Managerassociation : public QDialog
{
    Q_OBJECT

public:
    explicit Managerassociation(QWidget *parent = 0);
    ~Managerassociation();

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
    Ui::Managerassociation *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
//    void MatchNewest(association *newest );

};

#endif // MANAGERassociation_H
