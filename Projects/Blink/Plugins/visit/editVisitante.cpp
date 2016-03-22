#include "editVisitante.h"
#include "ui_editVisitante.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


EditVisitante::EditVisitante(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditVisitante)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

EditVisitante::~EditVisitante()
{
    delete ui;
}
void EditVisitante::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void EditVisitante::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void EditVisitante::SetModel(Visitante* mod)
{
   m_mod = mod;
   Load();
}


void EditVisitante::Save()
{

    Visitante* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Visitante;

   // mod->setNome(ui->LnEdtNome->text());
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

void EditVisitante::Load()
{
    if( m_mod == NULL)
      return;
  //  ui->LnEdtNome->setText(m_mod->getNome());

}

void EditVisitante::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Visitante* EditVisitante::GetSaved()
{
   return m_lastMod;

}

