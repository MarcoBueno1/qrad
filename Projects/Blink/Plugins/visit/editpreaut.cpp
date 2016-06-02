#include "editpreaut.h"
#include "ui_editpreaut.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editpreaut::Editpreaut(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editpreaut)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxvisit->setTable("visitante");
    ui->CmbBxvisit->setField("Nome");
    ui->CmbBxvisit->setCanAdd(true);
    ui->CmbBxvisit->setUserName("QRad");
    ui->CmbBxvisit->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxauthorizer->setTable("dweller");
    ui->CmbBxauthorizer->setField("Name");
    ui->CmbBxauthorizer->setCanAdd(true);
    ui->CmbBxauthorizer->setUserName("QRad");
    ui->CmbBxauthorizer->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxreason->setTable("reason");
    ui->CmbBxreason->setField("Description");
    ui->CmbBxreason->setCanAdd(true);
    ui->CmbBxreason->setUserName("QRad");
    ui->CmbBxreason->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editpreaut::~Editpreaut()
{
    delete ui;
}
void Editpreaut::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editpreaut::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editpreaut::SetModel(preaut* mod)
{
   m_mod = mod;
   Load();
}


void Editpreaut::Save()
{

    preaut* mod =  m_mod;
    if( m_mod == NULL)
        mod = new preaut;

    mod->setsunday(ui->ChkBxsunday->isChecked());
    mod->setmonday(ui->ChkBxmonday->isChecked());
    mod->settuesday(ui->ChkBxtuesday->isChecked());
    mod->setwednesday(ui->ChkBxwednesday->isChecked());
    mod->setthursday(ui->ChkBxthursday->isChecked());
    mod->setfriday(ui->ChkBxfriday->isChecked());
    mod->setsaturday(ui->ChkBxsaturday->isChecked());
    mod->setvisit(ui->CmbBxvisit->model()->data(ui->CmbBxvisit->model()->index(ui->CmbBxvisit->currentIndex(), 0)).toInt());

    mod->sethoraini(ui->TmEdthoraini->time());
    mod->sethorafim(ui->TmEdthorafim->time());
    mod->setvalidate(ui->DtEdtvalidate->date());
    mod->setauthorizer(ui->CmbBxauthorizer->model()->data(ui->CmbBxauthorizer->model()->index(ui->CmbBxauthorizer->currentIndex(), 0)).toInt());

    mod->setobs(ui->LnEdtobs->text());
    mod->setautsince(ui->DtEdtautsince->date());
    mod->setreason(ui->CmbBxreason->model()->data(ui->CmbBxreason->model()->index(ui->CmbBxreason->currentIndex(), 0)).toInt());

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

void Editpreaut::Load()
{
    if( m_mod == NULL)
      return;
    ui->ChkBxsunday->setChecked(m_mod->getsunday());
    ui->ChkBxmonday->setChecked(m_mod->getmonday());
    ui->ChkBxtuesday->setChecked(m_mod->gettuesday());
    ui->ChkBxwednesday->setChecked(m_mod->getwednesday());
    ui->ChkBxthursday->setChecked(m_mod->getthursday());
    ui->ChkBxfriday->setChecked(m_mod->getfriday());
    ui->ChkBxsaturday->setChecked(m_mod->getsaturday());
    ui->CmbBxvisit->setCurrentId(m_mod->getvisit());
    ui->TmEdthoraini->setTime(m_mod->gethoraini());
    ui->TmEdthorafim->setTime(m_mod->gethorafim());
    ui->DtEdtvalidate->setDate(m_mod->getvalidate());
    ui->CmbBxauthorizer->setCurrentId(m_mod->getauthorizer());
    ui->LnEdtobs->setText(m_mod->getobs());
    ui->DtEdtautsince->setDate(m_mod->getautsince());
    ui->CmbBxreason->setCurrentId(m_mod->getreason());

}

void Editpreaut::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
preaut* Editpreaut::GetSaved()
{
   return m_lastMod;

}

