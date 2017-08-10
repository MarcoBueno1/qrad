#include "ticketpayment.h"
#include "ui_ticketpayment.h"
#include <QMessageBox>

TicketPayment::TicketPayment(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TicketPayment)
{
    ui->setupUi(this);

    setWindowTitle("Pagamento Manual");

    ui->dateEdit->setDate(QDate::currentDate());

    connect(ui->pushButtonOk, SIGNAL(clicked(bool)),this,SLOT(OKClicked()));
    connect(ui->pushButtonCancelar, SIGNAL(clicked(bool)),this, SLOT(reject()));
}

TicketPayment::~TicketPayment()
{
    delete ui;
}

void TicketPayment::OKClicked()
{
    if( ui->doubleSpinBox->value() <= 0.00)
    {
        QMessageBox::warning(this,"Oops!", "O Campo Valor não pode ficar vazio!");
        ui->doubleSpinBox->setFocus();
        return;
    }

    if( ui->lineEdit->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this,"Oops!", "O Campo Motivo não pode ficar vazio!");
        ui->lineEdit->setFocus();
        return;
    }
    accept();
}

double TicketPayment::getValue()
{
    return ui->doubleSpinBox->value();
}

QDate TicketPayment::getDate()
{
    return ui->dateEdit->date();
}

QString TicketPayment::getReason()
{
    return ui->lineEdit->text();
}

void TicketPayment::setInformations(double value, QString Caption)
{
    ui->doubleSpinBox->setValue(value);
    setWindowTitle(windowTitle() + QString(" (%1)").arg(Caption));
}

