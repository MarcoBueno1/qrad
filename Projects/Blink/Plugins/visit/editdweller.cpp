#include "editdweller.h"
#include "ui_editDweller.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


EditDweller::EditDweller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDweller)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
	
    ui->comboBoxAp->setTable("ap");
    ui->comboBoxAp->setField("Numero");
    ui->comboBoxAp->setCanAdd(true);
    ui->comboBoxAp->setUserName("QRad");
    ui->comboBoxAp->completer()->setFilterMode(Qt::MatchContains );

    ui->comboBoxApTorre->setTable("tower");
    ui->comboBoxApTorre->setField("Name");
    ui->comboBoxApTorre->setCanAdd(true);
    ui->comboBoxApTorre->setUserName("QRad");
    ui->comboBoxApTorre->completer()->setFilterMode(Qt::MatchContains );
	
    ui->comboBoxTipo->setTable("dweller_type");
    ui->comboBoxTipo->setField("Name");
    ui->comboBoxTipo->setCanAdd(true);
    ui->comboBoxTipo->setUserName("QRad");
    ui->comboBoxTipo->completer()->setFilterMode(Qt::MatchContains );

	ui->comboBoxProfissao->setTable("jobtitle");
    ui->comboBoxProfissao->setField("Name");
    ui->comboBoxProfissao->setCanAdd(true);
    ui->comboBoxProfissao->setUserName("QRad");
    ui->comboBoxProfissao->completer()->setFilterMode(Qt::MatchContains );

}

EditDweller::~EditDweller()
{
    delete ui;
}
void EditDweller::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void EditDweller::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void EditDweller::SetModel(Dweller* mod)
{
   m_mod = mod;
   Load();
}


void EditDweller::Save()
{

    Dweller* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Dweller;

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

void EditDweller::Load()
{
    if( m_mod == NULL)
      return;
  //  ui->LnEdtNome->setText(m_mod->getNome());

}

void EditDweller::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Dweller* EditDweller::GetSaved()
{
   return m_lastMod;

}

