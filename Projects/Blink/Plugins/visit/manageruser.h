#ifndef MANAGERuser_H
#define MANAGERuser_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "edituser.h"

namespace Ui {
class Manageruser;
}

class Manageruser : public QDialog
{
    Q_OBJECT

public:
    explicit Manageruser(QWidget *parent = 0);
    ~Manageruser();

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
    Ui::Manageruser *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(user *newest );

};

#endif // MANAGERuser_H
