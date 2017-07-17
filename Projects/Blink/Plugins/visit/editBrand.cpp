#include "editBrand.h"
#include "ui_editBrand.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


EditBrand::EditBrand(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditBrand)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));

    setWindowTitle("Cadastro de Marcas");
}

EditBrand::~EditBrand()
{
    delete ui;
}
void EditBrand::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void EditBrand::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void EditBrand::SetModel(Brand* mod)
{
   m_mod = mod;
   Load();
}


void EditBrand::Save()
{

    Brand* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Brand;

    mod->setName(ui->LnEdtName->text());
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

void EditBrand::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtName->setText(m_mod->getName());

}

void EditBrand::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Brand* EditBrand::GetSaved()
{
   return m_lastMod;

}

