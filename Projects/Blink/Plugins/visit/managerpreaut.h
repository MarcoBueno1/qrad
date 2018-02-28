#ifndef MANAGERpreaut_H
#define MANAGERpreaut_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editpreaut.h"

namespace Ui {
class Managerpreaut;
}

class Managerpreaut : public QDialog
{
    Q_OBJECT

public:
    explicit Managerpreaut(QWidget *parent = 0);
    ~Managerpreaut();

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

    void doExcluir();
private:
    Ui::Managerpreaut *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(preaut *newest );

};

#endif // MANAGERpreaut_H
