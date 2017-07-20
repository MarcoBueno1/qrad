#ifndef MANAGERassociation_H
#define MANAGERassociation_H

#include <QTimer>
#include <QDialog>
#include <QKeyEvent>
#include <QSqlQueryModel>

typedef enum
{
    tpNone,
    tpAccountToReceive,
    tpAccountToPay,
    tpdweller,
    tpsupplier,
    tpuser,
    tpVisit
}QRAD_ASSOCIATION_TYPE, *PQRAD_ASSOCIATION_TYPE;

namespace Ui {
class Managerassociation;
}

class Managerassociation : public QDialog
{
    Q_OBJECT

public:
    explicit Managerassociation(QWidget *parent = 0);
    ~Managerassociation();
    void setSQL(QString SQL, QRAD_ASSOCIATION_TYPE type);
    int getSelectedId();

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
    QString SQL_ITEMS;
    QRAD_ASSOCIATION_TYPE m_association;
    int m_associatedid;
    Ui::Managerassociation *ui;

    QTimer *m_keyinterval;
    QSqlQueryModel  *m_Model;

    void ShowCurrentInformations( void );
    void LoadTableView();
    void DoRefresh();
    void refreshTable();
    void ConfigureTable();
    void keyPressEvent(QKeyEvent *event);
};

#endif // MANAGERassociation_H
