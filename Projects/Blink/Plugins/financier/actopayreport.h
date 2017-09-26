#ifndef ACTOPAYREPORT_H
#define ACTOPAYREPORT_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class actopayreport;
}

class actopayreport : public QDialog
{
    Q_OBJECT

public:
    explicit actopayreport(QWidget *parent = 0);
    ~actopayreport();

public slots:
    void BuildReport(bool);

private:
    Ui::actopayreport *ui;
    QSqlQueryModel *m_Fornecedor;
    QSqlQueryModel *m_AccountType;
    QString MountSQLReport();
};

#endif // ACTOPAYREPORT_H
