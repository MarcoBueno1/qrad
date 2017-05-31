#ifndef MANAGERmetreage_H
#define MANAGERmetreage_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editmetreage.h"

namespace Ui {
class Managermetreage;
}

class Managermetreage : public QDialog
{
    Q_OBJECT

public:
    explicit Managermetreage(QWidget *parent = 0);
    ~Managermetreage();

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
    Ui::Managermetreage *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(metreage *newest );

};

#endif // MANAGERmetreage_H
