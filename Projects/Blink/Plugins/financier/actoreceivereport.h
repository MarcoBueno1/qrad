#ifndef ACTORECEIVEREPORT_H
#define ACTORECEIVEREPORT_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class actoReceiveReport;
}

class actoReceiveReport : public QDialog
{
    Q_OBJECT

public:
    explicit actoReceiveReport(QWidget *parent = 0);
    ~actoReceiveReport();

public slots:
    void ManageTxExtra();
    void ManagePessoaAP();
    void BuildReport(bool);
private:
    Ui::actoReceiveReport *ui;
    QSqlQueryModel *m_pessoaAP;
    QSqlQueryModel *m_AccountType;
    QString MountSQLReport();
};

#endif // ACTORECEIVEREPORT_H
