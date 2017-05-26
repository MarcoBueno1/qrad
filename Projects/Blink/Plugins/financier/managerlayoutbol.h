#ifndef MANAGERlayoutbol_H
#define MANAGERlayoutbol_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editlayoutbol.h"

namespace Ui {
class Managerlayoutbol;
}

class Managerlayoutbol : public QDialog
{
    Q_OBJECT

public:
    explicit Managerlayoutbol(QWidget *parent = 0);
    ~Managerlayoutbol();

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
    Ui::Managerlayoutbol *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(layoutbol *newest );

};

#endif // MANAGERlayoutbol_H
