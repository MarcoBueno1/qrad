#include "editemail.h"
#include "ui_editemail.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editemail::Editemail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editemail)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editemail::~Editemail()
{
    delete ui;
}
void Editemail::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editemail::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editemail::SetModel(email* mod)
{
   m_mod = mod;
   Load();
}


void Editemail::Save()
{

    email* mod =  m_mod;
    if( m_mod == NULL)
        mod = new email;

    mod->setuser(ui->LnEdtuser->text());
    mod->setpassword(ui->LnEdtpassword->text());
    mod->sethost(ui->LnEdthost->text());
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

void Editemail::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtuser->setText(m_mod->getuser());
    ui->LnEdtpassword->setText(m_mod->getpassword());
    ui->LnEdthost->setText(m_mod->gethost());
    ui->LnEdtport->setText(m_mod->getport());

}

void Editemail::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
email* Editemail::GetSaved()
{
   return m_lastMod;

}

