#include "editlogin.h"
#include "ui_editlogin.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QCryptographicHash>
#include "user.h"
#include "profile.h"


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

   if( ui->LnEdtemail->text().trimmed().isEmpty())
   {
     QMessageBox::warning(this, "Oops","O campo email não pode ficar vazio!"); 
     ui->LnEdtemail->selectAll();
     ui->LnEdtemail->setFocus();
     return;
   }
   if( ui->LnEdtpassword->text().trimmed().isEmpty())
   {
     QMessageBox::warning(this, "Oops","O campo senha não pode ficar vazio!"); 
     ui->LnEdtpassword->selectAll();
     ui->LnEdtpassword->setFocus();
     return;
   }
   QVariant pass = ui->LnEdtpassword->text().toLower();
   QString md5 = QCryptographicHash::hash(pass.toByteArray(), QCryptographicHash::Md5).toHex();
   user *usr = user::findByPassword(ui->LnEdtemail->text(), md5);

   if(! usr )
   {
     QMessageBox::warning(this, "Oops","Usuario ou Senha nao conferem!"); 
     ui->LnEdtpassword->selectAll();
     ui->LnEdtpassword->setFocus();
     return;
   }

   profile *pro = profile::findByid( usr->getprofile());
   if(!pro)
   {
     QMessageBox::warning(this, "Oops","Perfil do usuário não encontrado!"); 
     return;
   }

   QCoreApplication *app = QCoreApplication::instance();
   app->setProperty("CurrentUserId", usr->getid());
   app->setProperty("usepreaut", pro->getusepreaut());
   app->setProperty("usevisit", pro->getusevisit());
   app->setProperty("useuser", pro->getuseuser());
   app->setProperty("useprofile", pro->getuseprofile());
   app->setProperty("usedweller", pro->getusedweller());
   app->setProperty("useivisitant", pro->getusevisitant());

    login* mod =  m_mod;
    if( m_mod == NULL)
        mod = new login;

    mod->setuser(usr->getid());
    mod->setdate(QDate::currentDate());
    mod->settime(QTime::currentTime());
    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;

    delete usr;
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

