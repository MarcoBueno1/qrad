#ifndef MANAGERcnab_H
#define MANAGERcnab_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editcnab.h"

namespace Ui {
class Managercnab;
}

class Managercnab : public QDialog
{
    Q_OBJECT

public:
    explicit Managercnab(QWidget *parent = 0);
    ~Managercnab();

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
    Ui::Managercnab *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(cnab *newest );

};

#endif // MANAGERcnab_H
