#include "editrespemis.h"
#include "ui_editrespemis.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editrespemis::Editrespemis(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editrespemis)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editrespemis::~Editrespemis()
{
    delete ui;
}
void Editrespemis::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editrespemis::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editrespemis::SetModel(respemis* mod)
{
   m_mod = mod;
   Load();
}


void Editrespemis::Save()
{

    respemis* mod =  m_mod;
    if( m_mod == NULL)
        mod = new respemis;

    mod->setDescription(ui->LnEdtDescription->text());
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

void Editrespemis::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtDescription->setText(m_mod->getDescription());

}

void Editrespemis::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
respemis* Editrespemis::GetSaved()
{
   return m_lastMod;

}

