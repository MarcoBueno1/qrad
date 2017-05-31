#include "editmetreage.h"
#include "ui_editmetreage.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editmetreage::Editmetreage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editmetreage)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editmetreage::~Editmetreage()
{
    delete ui;
}
void Editmetreage::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editmetreage::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editmetreage::SetModel(metreage* mod)
{
   m_mod = mod;
   Load();
}


void Editmetreage::Save()
{

    metreage* mod =  m_mod;
    if( m_mod == NULL)
        mod = new metreage;

    mod->setDescricao(ui->LnEdtDescricao->text());
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

void Editmetreage::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtDescricao->setText(m_mod->getDescricao());

}

void Editmetreage::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
metreage* Editmetreage::GetSaved()
{
   return m_lastMod;

}

