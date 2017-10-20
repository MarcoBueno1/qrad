#ifndef CHOOSEREPORTRESERVE_H
#define CHOOSEREPORTRESERVE_H

#include <QDialog>
#include <QSqlQueryModel>
#include <QDate>

namespace Ui {
class ChooseReportReserve;
}

class ChooseReportReserve : public QDialog
{
    Q_OBJECT

public:
    explicit ChooseReportReserve(QWidget *parent = 0);
    ~ChooseReportReserve();

public slots:
    void dateChanged(QDate);
    void BuildReport(bool);
private:
    Ui::ChooseReportReserve *ui;
    QSqlQueryModel *m_modelLocal;
    QSqlQueryModel *m_modelPessoa;
    void FillComboPerson();
};

#endif // CHOOSEREPORTRESERVE_H
