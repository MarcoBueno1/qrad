#include "editlayoutbol.h"
#include "ui_editlayoutbol.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editlayoutbol::Editlayoutbol(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editlayoutbol)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editlayoutbol::~Editlayoutbol()
{
    delete ui;
}
void Editlayoutbol::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editlayoutbol::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editlayoutbol::SetModel(layoutbol* mod)
{
   m_mod = mod;
   Load();
}


void Editlayoutbol::Save()
{

    layoutbol* mod =  m_mod;
    if( m_mod == NULL)
        mod = new layoutbol;

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

void Editlayoutbol::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtDescription->setText(m_mod->getDescription());

}

void Editlayoutbol::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
layoutbol* Editlayoutbol::GetSaved()
{
   return m_lastMod;

}

