#include "editmaincompany.h"
#include "ui_editmaincompany.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editmaincompany::Editmaincompany(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editmaincompany)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxcity->setTable("city");
    ui->CmbBxcity->setField("name");
    ui->CmbBxcity->setCanAdd(true);
    ui->CmbBxcity->setUserName("dsm");
    ui->CmbBxcity->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxstate->setTable("state");
    ui->CmbBxstate->setField("name");
    ui->CmbBxstate->setCanAdd(true);
    ui->CmbBxstate->setUserName("dsm");
    ui->CmbBxstate->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editmaincompany::~Editmaincompany()
{
    delete ui;
}
void Editmaincompany::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editmaincompany::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editmaincompany::SetModel(maincompany* mod)
{
   m_mod = mod;
   Load();
}


void Editmaincompany::Save()
{

    maincompany* mod =  m_mod;
    if( m_mod == NULL)
        mod = new maincompany;

    mod->setname(ui->LnEdtname->text());
    mod->setfantasyname(ui->LnEdtfantasyname->text());
    mod->setcnpj(ui->LnEdtcnpj->text());
    mod->setie(ui->LnEdtie->text());
    mod->setphone(ui->LnEdtphone->text());
    mod->setemail(ui->LnEdtemail->text());
    mod->setlicense(ui->LnEdtlicense->text());
    mod->setcity(ui->CmbBxcity->model()->data(ui->CmbBxcity->model()->index(ui->CmbBxcity->currentIndex(), 0)).toInt());

    mod->setstate(ui->CmbBxstate->model()->data(ui->CmbBxstate->model()->index(ui->CmbBxstate->currentIndex(), 0)).toInt());

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

void Editmaincompany::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtname->setText(m_mod->getname());
    ui->LnEdtfantasyname->setText(m_mod->getfantasyname());
    ui->LnEdtcnpj->setText(m_mod->getcnpj());
    ui->LnEdtie->setText(m_mod->getie());
    ui->LnEdtphone->setText(m_mod->getphone());
    ui->LnEdtemail->setText(m_mod->getemail());
    ui->LnEdtlicense->setText(m_mod->getlicense());
    ui->CmbBxcity->setCurrentId(m_mod->getcity());
    ui->CmbBxstate->setCurrentId(m_mod->getstate());

}

void Editmaincompany::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
maincompany* Editmaincompany::GetSaved()
{
   return m_lastMod;

}

