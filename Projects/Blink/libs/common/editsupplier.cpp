#include "editsupplier.h"
#include "ui_editsupplier.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editsupplier::Editsupplier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editsupplier)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editsupplier::~Editsupplier()
{
    delete ui;
}
void Editsupplier::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editsupplier::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editsupplier::SetModel(supplier* mod)
{
   m_mod = mod;
   Load();
}


void Editsupplier::Save()
{

    supplier* mod =  m_mod;
    if( m_mod == NULL)
        mod = new supplier;

    mod->setCNPJ(ui->LnEdtCNPJ->text());
    mod->setNome(ui->LnEdtNome->text());
    mod->setFantasia(ui->LnEdtFantasia->text());
    mod->setData(ui->DtEdtData->date());
    mod->setHora(ui->TmEdtHora->time());
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

void Editsupplier::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtCNPJ->setText(m_mod->getCNPJ());
    ui->LnEdtNome->setText(m_mod->getNome());
    ui->LnEdtFantasia->setText(m_mod->getFantasia());
    ui->DtEdtData->setDate(m_mod->getData());
    ui->TmEdtHora->setTime(m_mod->getHora());

}

void Editsupplier::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
supplier* Editsupplier::GetSaved()
{
   return m_lastMod;

}

