#ifndef MANAGERBrand_H
#define MANAGERBrand_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editBrand.h"

namespace Ui {
class ManagerBrand;
}

class ManagerBrand : public QDialog
{
    Q_OBJECT

public:
    explicit ManagerBrand(QWidget *parent = 0);
    ~ManagerBrand();

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
    Ui::ManagerBrand *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(Brand *newest );

};

#endif // MANAGERBrand_H
