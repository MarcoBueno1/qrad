#include "editlogin.h"
#include "ui_editlogin.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editlogin::Editlogin(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editlogin)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editlogin::~Editlogin()
{
    delete ui;
}
void Editlogin::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editlogin::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editlogin::SetModel(login* mod)
{
   m_mod = mod;
   Load();
}


void Editlogin::Save()
{

    login* mod =  m_mod;
    if( m_mod == NULL)
        mod = new login;

    mod->setuser(ui->SpnBxuser->value());
    mod->setdate(ui->DtEdtdate->date());
    mod->settime(ui->TmEdttime->time());
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

void Editlogin::Load()
{
    if( m_mod == NULL)
      return;
    ui->SpnBxuser->setValue(m_mod->getuser());
    ui->DtEdtdate->setDate(m_mod->getdate());
    ui->TmEdttime->setTime(m_mod->gettime());

}

void Editlogin::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
login* Editlogin::GetSaved()
{
   return m_lastMod;

}

