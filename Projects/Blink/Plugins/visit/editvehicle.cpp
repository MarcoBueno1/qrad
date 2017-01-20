#include "editvehicle.h"
#include "ui_editvehicle.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>

// qrad -s vehicle -t vehicle -c id -i int -c owner -i int -c board -i QString -c brand -i int:multi:Brand.Name[Toyota,Chevrolet,Fiat,Volkswagen, Ford, Honda] -c veicname -i int:multi:veicname.type[Corolla,Civic,Jetta]

Editvehicle::Editvehicle(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editvehicle)
{
    ui->setupUi(this);

    m_owner = 0;
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxbrand->setTable("Brand");
    ui->CmbBxbrand->setField("Name");
    ui->CmbBxbrand->setCanAdd(true);
    ui->CmbBxbrand->setUserName("dsm");

    ui->CmbBxbrand->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxveicname->setTable("veicname");
    ui->CmbBxveicname->setField("type");
    ui->CmbBxveicname->setCanAdd(true);
    ui->CmbBxveicname->setUserName("dsm");
    ui->CmbBxveicname->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

void Editvehicle::setOwner(int owner)
{
    m_owner = owner;
}

Editvehicle::~Editvehicle()
{
    delete ui;
}
void Editvehicle::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editvehicle::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editvehicle::SetModel(vehicle* mod)
{
   m_mod = mod;
   Load();
}


void Editvehicle::Save()
{

    vehicle* mod =  m_mod;
    if( m_mod == NULL)
        mod = new vehicle;

    mod->setowner(m_owner);
    mod->setboard(ui->LnEdtboard->text());
    mod->setbrand(ui->CmbBxbrand->model()->data(ui->CmbBxbrand->model()->index(ui->CmbBxbrand->currentIndex(), 0)).toInt());

    mod->setveicname(ui->CmbBxveicname->model()->data(ui->CmbBxveicname->model()->index(ui->CmbBxveicname->currentIndex(), 0)).toInt());

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

void Editvehicle::Load()
{
    if( m_mod == NULL)
      return;

    m_owner = m_mod->getowner();
    ui->LnEdtboard->setText(m_mod->getboard());
    ui->CmbBxbrand->setCurrentId(m_mod->getbrand());
    ui->CmbBxveicname->setCurrentId(m_mod->getveicname());

}

void Editvehicle::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
vehicle* Editvehicle::GetSaved()
{
   return m_lastMod;

}

