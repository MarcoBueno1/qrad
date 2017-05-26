#ifndef MANAGERsupplier_H
#define MANAGERsupplier_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editsupplier.h"

namespace Ui {
class Managersupplier;
}

class Managersupplier : public QDialog
{
    Q_OBJECT

public:
    explicit Managersupplier(QWidget *parent = 0);
    ~Managersupplier();

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
    Ui::Managersupplier *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(supplier *newest );

};

#endif // MANAGERsupplier_H
