#ifndef MANAGERcommon_area_H
#define MANAGERcommon_area_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editcommon_area.h"

namespace Ui {
class Managercommon_area;
}

class Managercommon_area : public QDialog
{
    Q_OBJECT

public:
    explicit Managercommon_area(QWidget *parent = 0);
    ~Managercommon_area();

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
    Ui::Managercommon_area *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(common_area *newest );

};

#endif // MANAGERcommon_area_H
