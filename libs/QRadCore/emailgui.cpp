#include "emailgui.h"
#include "ui_emailgui.h"

EmailGui::EmailGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGui)
{
    ui->setupUi(this);
    setWindowTitle("Enviar e-mail...");
}

EmailGui::~EmailGui()
{
    delete ui;
}
void EmailGui::setText(QString text)
{
    ui->textEdit->setText(text);
}

void EmailGui::setTo(QString to, bool enabled)
{
    ui->lineEditPara->setText(to);
    ui->lineEditPara->setReadOnly(!enabled);
}

void EmailGui::setSubject(QString Subject, bool enabled)
{
    ui->lineEditAssunto->setText(Subject);
    ui->lineEditAssunto->setReadOnly(!enabled);

}

QString EmailGui::getText()
{
    return ui->textEdit->document()->toPlainText();
}
void EmailGui::setFile(QString file)
{
    ui->LblFile->setText(file);
}
