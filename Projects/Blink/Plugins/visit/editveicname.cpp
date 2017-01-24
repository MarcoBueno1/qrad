#include "editveicname.h"
#include "ui_editveicname.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editveicname::Editveicname(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editveicname)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editveicname::~Editveicname()
{
    delete ui;
}
void Editveicname::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editveicname::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editveicname::SetModel(veicname* mod)
{
   m_mod = mod;
   Load();
}


void Editveicname::Save()
{

    veicname* mod =  m_mod;
    if( m_mod == NULL)
        mod = new veicname;

    mod->settype(ui->LnEdttype->text());
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

void Editveicname::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdttype->setText(m_mod->gettype());

}

void Editveicname::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
veicname* Editveicname::GetSaved()
{
   return m_lastMod;

}

