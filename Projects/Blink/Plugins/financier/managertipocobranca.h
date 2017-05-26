#ifndef MANAGERtipocobranca_H
#define MANAGERtipocobranca_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "edittipocobranca.h"

namespace Ui {
class Managertipocobranca;
}

class Managertipocobranca : public QDialog
{
    Q_OBJECT

public:
    explicit Managertipocobranca(QWidget *parent = 0);
    ~Managertipocobranca();

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
    Ui::Managertipocobranca *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(tipocobranca *newest );

};

#endif // MANAGERtipocobranca_H
