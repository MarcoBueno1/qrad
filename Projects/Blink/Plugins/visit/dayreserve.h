#ifndef DAYRESERVE_H
#define DAYRESERVE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDate>
#include "column2delegate.h"

namespace Ui {
class DayReserve;
}

class DayReserve : public QDialog
{
    Q_OBJECT

public:
    explicit DayReserve(QWidget *parent = 0);
    ~DayReserve();

    int Exec(int CommonId, QString CommonName, QDate day);
public slots:
    void NewClicked(bool);
    void CenceledClicked(bool);
    void EditClicked(bool);
private:
    Ui::DayReserve *ui;
    QSqlQueryModel *m_model;
    int m_CommonId;
    QString m_CommonName;
    QDate m_day;
    ColumnCenter *m_CenterDelegate;
    ColumnLeft *m_LeftDelegate;
    void RunSQL();
};

#endif // DAYRESERVE_H
