#ifndef ENTERBILLET_H
#define ENTERBILLET_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QKeyEvent>
#include "qradshared.h"
#include "dsmsgmessages.h"
#include "entrancemodel.h"
#include "accounttopayhistorymodel.h"

namespace Ui {
    class EnterBillet;
}

class EnterBillet : public QDialog {
    Q_OBJECT
public:
    EnterBillet(QWidget *parent = 0);
    ~EnterBillet();
    void    GetEntranceId(int entranceId);

protected:
    void    changeEvent(QEvent *e);
    void    showEvent(QShowEvent*);
    void    closeEvent(QCloseEvent*);
    void    keyPressEvent( QKeyEvent* );
    void    ConfigureHeaderTableLotsIndicated(void);
    void    UpdateTable(void);
    void    ClearTable(void);
    void    ClearFields(void);
    void    RemoveLastItem(void);
    double  GetTotalAmount(DSM_Lot_t lotList);

private:
    Ui::EnterBillet *m_ui;

    int                 m_entranceId,
                        m_supplierId;
    double              m_entranceTotalValue, m_totalPaid;
    QDate               m_entranceDate;
    QList<DSM_Billet_t> m_billetList;

private slots:
    void    AddBillet(void);
    void    changeData(QModelIndex indexA, QModelIndex indexB);
};

#endif // ENTERBILLET_H
