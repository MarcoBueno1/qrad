#ifndef MANAGERtipopessoa_H
#define MANAGERtipopessoa_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "edittipopessoa.h"

namespace Ui {
class Managertipopessoa;
}

class Managertipopessoa : public QDialog
{
    Q_OBJECT

public:
    explicit Managertipopessoa(QWidget *parent = 0);
    ~Managertipopessoa();

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
    Ui::Managertipopessoa *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(tipopessoa *newest );

};

#endif // MANAGERtipopessoa_H
