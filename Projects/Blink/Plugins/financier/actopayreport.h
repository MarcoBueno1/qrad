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

    void TestMountSQL();
public slots:
    void BuildReport(bool);

    void EnableDisableVencidas();
private:
    Ui::actopayreport *ui;
    QSqlQueryModel *m_Fornecedor;
    QSqlQueryModel *m_AccountType;
    QSqlQueryModel *m_paymentway;
    QString MountSQLReport();
};

#endif // ACTOPAYREPORT_H
