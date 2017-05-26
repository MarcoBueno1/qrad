#include "editbankaccount.h"
#include "ui_editbankaccount.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editbankaccount::Editbankaccount(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editbankaccount)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxBanco->setTable("fin_bank");
    ui->CmbBxBanco->setField("Description");
    ui->CmbBxBanco->setCanAdd(true);
    ui->CmbBxBanco->setUserName("QRad");
    ui->CmbBxBanco->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editbankaccount::~Editbankaccount()
{
    delete ui;
}
void Editbankaccount::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editbankaccount::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editbankaccount::SetModel(bankaccount* mod)
{
   m_mod = mod;
   Load();
}


void Editbankaccount::Save()
{

    bankaccount* mod =  m_mod;
    if( m_mod == NULL)
        mod = new bankaccount;

    mod->setConta(ui->LnEdtConta->text());
    mod->setDigitoConta(ui->LnEdtDigitoConta->text());
    mod->setAgencia(ui->LnEdtAgencia->text());
    mod->setDigitoAgencia(ui->LnEdtDigitoAgencia->text());
    mod->setBanco(ui->CmbBxBanco->model()->data(ui->CmbBxBanco->model()->index(ui->CmbBxBanco->currentIndex(), 0)).toInt());

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

void Editbankaccount::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtConta->setText(m_mod->getConta());
    ui->LnEdtDigitoConta->setText(m_mod->getDigitoConta());
    ui->LnEdtAgencia->setText(m_mod->getAgencia());
    ui->LnEdtDigitoAgencia->setText(m_mod->getDigitoAgencia());
    ui->CmbBxBanco->setCurrentId(m_mod->getBanco());

}

void Editbankaccount::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
bankaccount* Editbankaccount::GetSaved()
{
   return m_lastMod;

}

