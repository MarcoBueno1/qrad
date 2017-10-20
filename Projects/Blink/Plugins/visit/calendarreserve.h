#ifndef CALENDARRESERVE_H
#define CALENDARRESERVE_H

#include <QDialog>
#include <QDate>
#include <QSqlQueryModel>

namespace Ui {
class calendarreserve;
}

class calendarreserve : public QDialog
{
    Q_OBJECT

public:
    explicit calendarreserve(QWidget *parent = 0);
    ~calendarreserve();

    int Exec(int CommonId, QString CommonName);
public slots:
    void ShowDetail(QDate date);
    void UpdateDate();
    void UpdateCalendar(int year, int month);
private:
    Ui::calendarreserve *ui;
    QSqlQueryModel *m_model;
    int m_CommonId;
    QString m_CommonName;
};

#endif // CALENDARRESERVE_H
