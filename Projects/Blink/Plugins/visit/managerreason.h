#ifndef MANAGERreason_H
#define MANAGERreason_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editreason.h"

namespace Ui {
class Managerreason;
}

class Managerreason : public QDialog
{
    Q_OBJECT

public:
    explicit Managerreason(QWidget *parent = 0);
    ~Managerreason();

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
    Ui::Managerreason *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(reason *newest );

};

#endif // MANAGERreason_H
