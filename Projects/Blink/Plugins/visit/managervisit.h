#ifndef MANAGERvisit_H
#define MANAGERvisit_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editvisit.h"
#include "column2delegate.h"

namespace Ui {
class Managervisit;
}

class Managervisit : public QDialog
{
    Q_OBJECT

public:
    explicit Managervisit(QWidget *parent = 0);
    ~Managervisit();

    void resizeEvent(QResizeEvent *event);
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
    void LoadTableView();

private:
    Ui::Managervisit *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;
    ColumnDateTimeNull *m_DateNullDelagate;

    void ShowCurrentInformations( void );
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(visit *newest );

};

#endif // MANAGERvisit_H
