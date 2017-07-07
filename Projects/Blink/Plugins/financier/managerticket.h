#ifndef MANAGERticket_H
#define MANAGERticket_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>
#include "editticket.h"
#include <QResizeEvent>

namespace Ui {
class Managerticket;
}

class Managerticket : public QDialog
{
    Q_OBJECT

public:
    explicit Managerticket(QWidget *parent = 0);
    ~Managerticket();

//    void resizeEvent(QResizeEvent* event);
public slots:
    void notFound();
    void doRefresh();
    void Found( QModelIndex );
    void StartTimer( QString );
    void TableClicked( QModelIndex );
    void CurrentChanged( QModelIndex );
    void KeyPressTimeout();
    void doTxExtra();
    void doSair();
    void doTxCondominial();
    void doReprint();
    void doEdit();
    void doRemove();
    void doDrop(QString path);

    void doExport();
private:
    Ui::Managerticket *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
    void MatchNewest(ticket *newest );

    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
};

#endif // MANAGERticket_H
