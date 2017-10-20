#ifndef NEWDAYRESERVE_H
#define NEWDAYRESERVE_H

#include <QDialog>
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include "column2delegate.h"
#include "reserve.h"

namespace Ui {
class NewDayReserve;
}

class NewDayReserve : public QDialog
{
    Q_OBJECT

public:
    explicit NewDayReserve(QWidget *parent = 0);
    ~NewDayReserve();

    int Exec(int CommonId,  QString CommonName, QDate date);
    int Edit(int id, QString CommonName);
public slots:
    void Save(bool);
    void OnSelectionChanged(QItemSelection, QItemSelection);
private:
    Ui::NewDayReserve *ui;
    int m_CommonId;
    QString m_CommonName;
    QDate m_date;
    QSqlQueryModel *m_model;
    QSqlQueryModel *m_modelCombo;
    QSqlDatabase m_litedb;
    ColumnReserve *m_delegate;
    reserve *m_reserve;

    int RunTimes();
    QString GetResponsable(int dwellerid);
    bool CreateTempTable();
    void PopulateTableView();
    int InsertTemTbl(QTime timestart, QTime timeend, QString responsable);
    void FillComboBox();
};

#endif // NEWDAYRESERVE_H
