#include "edittipocobranca.h"
#include "ui_edittipocobranca.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Edittipocobranca::Edittipocobranca(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edittipocobranca)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Edittipocobranca::~Edittipocobranca()
{
    delete ui;
}
void Edittipocobranca::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Edittipocobranca::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Edittipocobranca::SetModel(tipocobranca* mod)
{
   m_mod = mod;
   Load();
}


void Edittipocobranca::Save()
{

    tipocobranca* mod =  m_mod;
    if( m_mod == NULL)
        mod = new tipocobranca;

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

void Edittipocobranca::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtDescription->setText(m_mod->getDescription());

}

void Edittipocobranca::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
tipocobranca* Edittipocobranca::GetSaved()
{
   return m_lastMod;

}

