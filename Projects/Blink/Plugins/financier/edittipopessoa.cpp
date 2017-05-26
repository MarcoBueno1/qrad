#include "edittipopessoa.h"
#include "ui_edittipopessoa.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Edittipopessoa::Edittipopessoa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Edittipopessoa)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Edittipopessoa::~Edittipopessoa()
{
    delete ui;
}
void Edittipopessoa::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Edittipopessoa::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Edittipopessoa::SetModel(tipopessoa* mod)
{
   m_mod = mod;
   Load();
}


void Edittipopessoa::Save()
{

    tipopessoa* mod =  m_mod;
    if( m_mod == NULL)
        mod = new tipopessoa;

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

void Edittipopessoa::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtDescription->setText(m_mod->getDescription());

}

void Edittipopessoa::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
tipopessoa* Edittipopessoa::GetSaved()
{
   return m_lastMod;

}

