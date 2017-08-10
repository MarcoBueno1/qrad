#ifndef TICKETPAYMENT_H
#define TICKETPAYMENT_H

#include <QDialog>

namespace Ui {
class TicketPayment;
}

class TicketPayment : public QDialog
{
    Q_OBJECT

public:
    explicit TicketPayment(QWidget *parent = 0);
    ~TicketPayment();

    double getValue();
    QDate getDate();
    QString getReason();
    void setInformations(double value, QString Caption);
public slots:
    void OKClicked();
private:
    Ui::TicketPayment *ui;
};

#endif // TICKETPAYMENT_H
