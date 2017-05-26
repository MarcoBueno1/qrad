#include "editcnab.h"
#include "ui_editcnab.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editcnab::Editcnab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editcnab)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editcnab::~Editcnab()
{
    delete ui;
}
void Editcnab::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editcnab::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editcnab::SetModel(cnab* mod)
{
   m_mod = mod;
   Load();
}


void Editcnab::Save()
{

    cnab* mod =  m_mod;
    if( m_mod == NULL)
        mod = new cnab;

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

void Editcnab::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtDescription->setText(m_mod->getDescription());

}

void Editcnab::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
cnab* Editcnab::GetSaved()
{
   return m_lastMod;

}

