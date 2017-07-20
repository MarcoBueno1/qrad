#include "connectservererror.h"
#include "ui_connectservererror.h"
#include "qradfile.h"

ConnectServerError::ConnectServerError(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ConnectServerError)
{
    ui->setupUi(this);
}

ConnectServerError::~ConnectServerError()
{
    delete ui;
}

void ConnectServerError::getError(QString text)
{
    ui->textEdit->setText(text);
}

void ConnectServerError::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    ui->lineEdit->setText(QRadFile::GetParamValue("banco/hostname"));
    ui->lineEdit->setFocus();
}

void ConnectServerError::on_btnExit_clicked()
{
    this->close();
}

void ConnectServerError::on_btnSave_clicked()
{
    QRadFile::SetParamValue("banco/hostname",ui->lineEdit->text());
    this->accept();
}
