#ifndef MANAGERVisitante_H
#define MANAGERVisitante_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editVisitante.h"

namespace Ui {
class ManagerVisitante;
}

class ManagerVisitante : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerVisitante(QWidget *parent = 0);
    ~ManagerVisitante();

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
    Ui::ManagerVisitante *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(Visitante *newest );

};

#endif // MANAGERVisitante_H
