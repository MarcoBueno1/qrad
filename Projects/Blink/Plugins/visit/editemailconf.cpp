#include "editemailconf.h"
#include "ui_editemailconf.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editemailconf::Editemailconf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editemailconf)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editemailconf::~Editemailconf()
{
    delete ui;
}
void Editemailconf::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editemailconf::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editemailconf::SetModel(emailconf* mod)
{
   m_mod = mod;
   Load();
}


void Editemailconf::Save()
{
    if( ui->LnEdtpassword->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite a senha!!");
        ui->LnEdtpassword->selectAll();
        ui->LnEdtpassword->setFocus();
        return ;
    }
    if( ui->LnEdtport->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite a porta!!");
        ui->LnEdtport->selectAll();
        ui->LnEdtport->setFocus();
        return ;

    }
    if( ui->LnEdtserver->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite o endereço do servidor!!");
        ui->LnEdtserver->selectAll();
        ui->LnEdtserver->setFocus();
        return ;

    }
    if( ui->LnEdtusername->text().trimmed().isEmpty())
    {
        QMessageBox::information(this, "Oops!", "Por favor, digite o usuário!!");
        ui->LnEdtusername->selectAll();
        ui->LnEdtusername->setFocus();
        return ;

    }

    emailconf* mod =  m_mod;
    if( m_mod == NULL)
        mod = new emailconf;

    mod->setusername(ui->LnEdtusername->text());
    mod->setpassword(ui->LnEdtpassword->text());
    mod->setserver(ui->LnEdtserver->text());
    mod->setport(ui->LnEdtport->text());
    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;
    if( bRet )
    {
       QMessageBox::information(this, "Sucesso!","Informações foram salvas com sucesso!");
       accept();
    }
    else
       QMessageBox::warning(this, "Oops","Não foi possivel salvar");
    
}

void Editemailconf::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtusername->setText(m_mod->getusername());
    ui->LnEdtpassword->setText(m_mod->getpassword());
    ui->LnEdtserver->setText(m_mod->getserver());
    ui->LnEdtport->setText(m_mod->getport());

}

void Editemailconf::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
emailconf* Editemailconf::GetSaved()
{
   return m_lastMod;

}

