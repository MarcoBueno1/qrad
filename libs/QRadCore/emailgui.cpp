#include "emailgui.h"
#include "ui_emailgui.h"
#include <QDesktopServices>
#include "qraddebug.h"

EmailGui::EmailGui(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EmailGui)
{
    ui->setupUi(this);
    setWindowTitle("Enviar e-mail...");
    connect(ui->pushButtonCancel,SIGNAL(clicked()),this,SLOT(reject()));
    connect(ui->pushButtonEnviar,SIGNAL(clicked()),this,SLOT(accept()));
    connect(ui->LblFile,SIGNAL(linkActivated(QString)),this,SLOT(doLink(QString)));

    ui->LblFile->setTextInteractionFlags(Qt::TextBrowserInteraction);
}
void EmailGui::doLink(const QString & link)
{
    debug_message("openUrl: %s\n", QString("file://%1").arg(ui->LblFile->text()).toLatin1().data());
    QDesktopServices::openUrl(QUrl(QString("file://%1").arg(ui->LblFile->text())));
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
