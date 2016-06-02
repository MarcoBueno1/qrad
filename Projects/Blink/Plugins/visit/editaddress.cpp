#include "editaddress.h"
#include "ui_editaddress.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editaddress::Editaddress(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editaddress)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    m_Owner = 0;
    m_OwnerType = 0;

    ui->CmbBxstreet->setTable("Street");
    ui->CmbBxstreet->setField("Name");
    ui->CmbBxstreet->setCanAdd(true);
    ui->CmbBxstreet->setUserName("QRad");
    ui->CmbBxstreet->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxcep->setTable("Cep");
    ui->CmbBxcep->setField("Number");
    ui->CmbBxcep->setCanAdd(true);
    ui->CmbBxcep->setUserName("QRad");
    ui->CmbBxcep->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxNeighborhood->setTable("Neighborhood");
    ui->CmbBxNeighborhood->setField("Name");
    ui->CmbBxNeighborhood->setCanAdd(true);
    ui->CmbBxNeighborhood->setUserName("QRad");
    ui->CmbBxNeighborhood->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxcity->setTable("City");
    ui->CmbBxcity->setField("Name");
    ui->CmbBxcity->setCanAdd(true);
    ui->CmbBxcity->setUserName("QRad");
    ui->CmbBxcity->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxstate->setTable("State");
    ui->CmbBxstate->setField("Name");
    ui->CmbBxstate->setCanAdd(true);
    ui->CmbBxstate->setUserName("QRad");
    ui->CmbBxstate->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editaddress::~Editaddress()
{
    delete ui;
}
void Editaddress::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editaddress::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editaddress::SetModel(Address* mod)
{
   m_mod = mod;
   Load();
}


void Editaddress::Save()
{
    Address* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Address;

    mod->setstreet(ui->CmbBxstreet->model()->data(ui->CmbBxstreet->model()->index(ui->CmbBxstreet->currentIndex(), 0)).toInt());

    mod->setnumber(ui->LnEdtnumber->text());
    mod->setcep(ui->CmbBxcep->model()->data(ui->CmbBxcep->model()->index(ui->CmbBxcep->currentIndex(), 0)).toInt());

    mod->setNeighborhood(ui->CmbBxNeighborhood->model()->data(ui->CmbBxNeighborhood->model()->index(ui->CmbBxNeighborhood->currentIndex(), 0)).toInt());

    mod->setcity(ui->CmbBxcity->model()->data(ui->CmbBxcity->model()->index(ui->CmbBxcity->currentIndex(), 0)).toInt());

    mod->setstate(ui->CmbBxstate->model()->data(ui->CmbBxstate->model()->index(ui->CmbBxstate->currentIndex(), 0)).toInt());

    mod->setOwner(m_Owner);
    mod->setOwnerType(m_OwnerType);

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

void Editaddress::Load()
{
    if( m_mod == NULL)
      return;
    ui->CmbBxstreet->setCurrentId(m_mod->getstreet());
    ui->LnEdtnumber->setText(m_mod->getnumber());
    ui->CmbBxcep->setCurrentId(m_mod->getcep());
    ui->CmbBxNeighborhood->setCurrentId(m_mod->getNeighborhood());
    ui->CmbBxcity->setCurrentId(m_mod->getcity());
    ui->CmbBxstate->setCurrentId(m_mod->getstate());

}

void Editaddress::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Address* Editaddress::GetSaved()
{
   return m_lastMod;

}

void Editaddress::setOwner(int id)
{
  m_Owner = id;
}
void Editaddress::setOwnerType(int id)
{
  m_OwnerType = id;
}

