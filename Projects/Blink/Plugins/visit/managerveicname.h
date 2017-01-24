#ifndef MANAGERveicname_H
#define MANAGERveicname_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editveicname.h"

namespace Ui {
class Managerveicname;
}

class Managerveicname : public QDialog
{
    Q_OBJECT

public:
    explicit Managerveicname(QWidget *parent = 0);
    ~Managerveicname();

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
    Ui::Managerveicname *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(veicname *newest );

};

#endif // MANAGERveicname_H
