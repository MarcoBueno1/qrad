#include "edituser.h"
#include "ui_edituser.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QCryptographicHash>

Edituser::Edituser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edituser)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;

    ui->CmbBxprofile->setTable("profile");
    ui->CmbBxprofile->setField("name");
    ui->CmbBxprofile->setCanAdd(false);
    ui->CmbBxprofile->setUserName("dsm");
    if( ui->CmbBxprofile->completer() )
          ui->CmbBxprofile->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonResetarSenha, SIGNAL(clicked()),this,SLOT(ResetSenha()));

    ui->pushButtonResetarSenha->setVisible(false);

    setWindowTitle("Cadastro de Usuários");
}

Edituser::~Edituser()
{
    delete ui;
}
void Edituser::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Edituser::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Edituser::SetModel(user* mod)
{
   m_mod = mod;
   Load();
}


void Edituser::Save()
{
   // consist

   if( ui->LnEdtname->text().trimmed().isEmpty())
   {
     QMessageBox::warning(this, "Oops","O campo nome não pode ficar vazio!"); 
     ui->LnEdtname->selectAll();
     ui->LnEdtname->setFocus();
     return;
   }
   if( ui->LnEdtemail->text().trimmed().isEmpty())
   {
     QMessageBox::warning(this, "Oops","O campo email não pode ficar vazio!"); 
     ui->LnEdtemail->selectAll();
     ui->LnEdtemail->setFocus();
     return;
   }
   if( ui->LnEdtpassword->isEnabled())
   {
       if( ui->LnEdtpassword->text().trimmed().isEmpty())
       {
         QMessageBox::warning(this, "Oops","O campo senha não pode ficar vazio!");
         ui->LnEdtpassword->selectAll();
         ui->LnEdtpassword->setFocus();
         return;
       }
       if( ui->LnEdtpassword->text() != ui->LnEdtConfirmPassword->text())
       {
         QMessageBox::warning(this, "Oops","O campo senha não confere com o campo confirmação de senha!");
         ui->LnEdtpassword->clear();
         ui->LnEdtConfirmPassword->clear();
         ui->LnEdtpassword->selectAll();
         ui->LnEdtpassword->setFocus();
         return;
       }
   }

   //

    user* mod =  m_mod;
    if( m_mod == NULL)
        mod = new user;

    mod->setname(ui->LnEdtname->text());
    mod->setemail(ui->LnEdtemail->text());
    mod->setcontractsince(ui->DtEdtcontractsince->date());

    if( ui->LnEdtpassword->isEnabled())
    {
        QVariant pass = ui->LnEdtpassword->text().toLower();
        QString md5 = QCryptographicHash::hash(pass.toByteArray(), QCryptographicHash::Md5).toHex();

        mod->setpassword(md5);
    }
    mod->setprofile(ui->CmbBxprofile->model()->data(ui->CmbBxprofile->model()->index(ui->CmbBxprofile->currentIndex(), 0)).toInt());

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

void Edituser::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtname->setText(m_mod->getname());
    ui->LnEdtemail->setText(m_mod->getemail());
    ui->DtEdtcontractsince->setDate(m_mod->getcontractsince());
    ui->CmbBxprofile->setCurrentId(m_mod->getprofile());
    ui->LnEdtpassword->setEnabled(false);
    ui->LnEdtConfirmPassword->setEnabled(false);

    ui->pushButtonResetarSenha->setVisible(true);

}

void Edituser::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
user* Edituser::GetSaved()
{
   return m_lastMod;

}

void Edituser::ResetSenha()
{
    ui->LnEdtpassword->setEnabled(true);
    ui->LnEdtConfirmPassword->setEnabled(true);
    ui->LnEdtpassword->setFocus();
}
