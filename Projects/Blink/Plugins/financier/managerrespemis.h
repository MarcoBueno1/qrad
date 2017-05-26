#ifndef MANAGERrespemis_H
#define MANAGERrespemis_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editrespemis.h"

namespace Ui {
class Managerrespemis;
}

class Managerrespemis : public QDialog
{
    Q_OBJECT

public:
    explicit Managerrespemis(QWidget *parent = 0);
    ~Managerrespemis();

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
    Ui::Managerrespemis *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(respemis *newest );

};

#endif // MANAGERrespemis_H
