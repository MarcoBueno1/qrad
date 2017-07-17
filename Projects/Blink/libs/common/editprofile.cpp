#include "editprofile.h"
#include "ui_editprofile.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editprofile::Editprofile(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editprofile)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));

    setWindowTitle("Edição de Perfil");
}

Editprofile::~Editprofile()
{
    delete ui;
}
void Editprofile::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editprofile::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editprofile::SetModel(profile* mod)
{
   m_mod = mod;
   Load();
}


void Editprofile::Save()
{

    profile* mod =  m_mod;
    if( m_mod == NULL)
        mod = new profile;

    mod->setname(ui->LnEdtname->text());
    mod->setusepreaut(ui->ChkBxusepreaut->isChecked());
    mod->setusevisit(ui->ChkBxusevisit->isChecked());
    mod->setusedweller(ui->ChkBxusedweller->isChecked());
    mod->setusevisitant(ui->ChkBxusevisitant->isChecked());
    mod->setuseuser(ui->ChkBxuseuser->isChecked());
    mod->setuseprofile(ui->ChkBxuseprofile->isChecked());
    mod->setConfigureEmail(ui->ChkConfigureemail->isChecked());
    mod->setGerarRelatorios(ui->ChkGerarRelatorios->isChecked());
    mod->setGeenciarBoletos(ui->ChkGerenciarBoletos->isChecked());
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

void Editprofile::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtname->setText(m_mod->getname());
    ui->ChkBxusepreaut->setChecked(m_mod->getusepreaut());
    ui->ChkBxusevisit->setChecked(m_mod->getusevisit());
    ui->ChkBxusedweller->setChecked(m_mod->getusedweller());
    ui->ChkBxusevisitant->setChecked(m_mod->getusevisitant());
    ui->ChkBxuseuser->setChecked(m_mod->getuseuser());
    ui->ChkBxuseprofile->setChecked(m_mod->getuseprofile());
    ui->ChkConfigureemail->setChecked(m_mod->getConfigureEmail());
    ui->ChkGerarRelatorios->setChecked(m_mod->getGerarRelatorios());
    ui->ChkGerenciarBoletos->setChecked(m_mod->getGeenciarBoletos());

}

void Editprofile::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
profile* Editprofile::GetSaved()
{
   return m_lastMod;

}

