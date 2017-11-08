#ifndef INVITEDS_H
#define INVITEDS_H

#include <QDialog>
#include <QSqlQueryModel>

namespace Ui {
class Inviteds;
}

class Inviteds : public QDialog
{
    Q_OBJECT

public:
    explicit Inviteds(QWidget *parent = 0);
    ~Inviteds();

    int Exec(int reserveId);
public slots:
    void NewInvited();
    void EditInvited();
    void RemoveInvited();
private:
    Ui::Inviteds *ui;
    int m_reserveId;
    QSqlQueryModel *m_model;
    void UpdateTable();
};

#endif // INVITEDS_H
