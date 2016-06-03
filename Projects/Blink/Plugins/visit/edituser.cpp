#include "edituser.h"
#include "ui_edituser.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Edituser::Edituser(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edituser)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxprofile->setTable("profile");
    ui->CmbBxprofile->setField("Name");
    ui->CmbBxprofile->setCanAdd(true);
    ui->CmbBxprofile->setUserName("QRad");
    ui->CmbBxprofile->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
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

    user* mod =  m_mod;
    if( m_mod == NULL)
        mod = new user;

    mod->setname(ui->LnEdtname->text());
    mod->setemail(ui->LnEdtemail->text());
    mod->setcontractsince(ui->DtEdtcontractsince->date());
    mod->setpassword(ui->LnEdtpassword->text());
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
    ui->LnEdtpassword->setText(m_mod->getpassword());
    ui->CmbBxprofile->setCurrentId(m_mod->getprofile());

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

