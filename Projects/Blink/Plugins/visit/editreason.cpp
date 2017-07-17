#include "editreason.h"
#include "ui_editreason.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editreason::Editreason(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editreason)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));

    setWindowTitle("Cadastro de Motivos de Visita");
}

Editreason::~Editreason()
{
    delete ui;
}
void Editreason::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editreason::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editreason::SetModel(reason* mod)
{
   m_mod = mod;
   Load();
}


void Editreason::Save()
{

    reason* mod =  m_mod;
    if( m_mod == NULL)
        mod = new reason;

    mod->setdescription(ui->LnEdtdescription->text());
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

void Editreason::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtdescription->setText(m_mod->getdescription());

}

void Editreason::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
reason* Editreason::GetSaved()
{
   return m_lastMod;

}

