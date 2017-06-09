#ifndef MANAGERshipper_H
#define MANAGERshipper_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
//#include "editshipper.h"

namespace Ui {
class Managershipper;
}

class Managershipper : public QDialog
{
    Q_OBJECT

public:
    explicit Managershipper(QWidget *parent = 0);
    ~Managershipper();

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
    Ui::Managershipper *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
//    void MatchNewest(shipper *newest );

};

#endif // MANAGERshipper_H
