#ifndef MANAGERfiles_H
#define MANAGERfiles_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editfiles.h"

namespace Ui {
class Managerfiles;
}

class Managerfiles : public QDialog
{
    Q_OBJECT

public:
    explicit Managerfiles(QWidget *parent = 0);
    ~Managerfiles();

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

private slots:
    void doRemove();
private:
    Ui::Managerfiles *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(Files *newest );

};

#endif // MANAGERfiles_H
