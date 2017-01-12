#ifndef REPORTLAUNCHER_H
#define REPORTLAUNCHER_H

#include <QDialog>
#include <QSqlQueryModel>
#include "ormreport.h"

namespace Ui {
class ReportLauncher;
}

class ReportLauncher : public QDialog
{
    Q_OBJECT

public:
    explicit ReportLauncher(QWidget *parent = 0);
    ~ReportLauncher();

    void Exec(QString action);
private slots:
    void OkPressed();
    void CancelPressed();
private:
    Ui::ReportLauncher *ui;
    report *m_report;
    QSqlQueryModel *m_model;
    void Buildreport();
    QStringList m_LeftHead;
    QStringList m_percents;

    void AutoSizeColumn(QSqlQueryModel *model);
    QString BuildSQL();
};

#endif // REPORTLAUNCHER_H
