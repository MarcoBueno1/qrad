#include "editticket.h"
#include "ui_editticket.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editticket::Editticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editticket)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxclientid->setTable("multi");
    ui->CmbBxclientid->setField("name");
    ui->CmbBxclientid->setCanAdd(true);
    ui->CmbBxclientid->setUserName("dsm");
    ui->CmbBxclientid->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxidticket->setTable("ticketconfig");
    ui->CmbBxidticket->setField("Description");
    ui->CmbBxidticket->setCanAdd(true);
    ui->CmbBxidticket->setUserName("dsm");
    ui->CmbBxidticket->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editticket::~Editticket()
{
    delete ui;
}
void Editticket::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editticket::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editticket::SetModel(ticket* mod)
{
   m_mod = mod;
   Load();
}


void Editticket::Save()
{

    ticket* mod =  m_mod;
    if( m_mod == NULL)
        mod = new ticket;

    mod->setNossoNumero(ui->LnEdtNossoNumero->text().toInt());
    mod->setSeuNumero(ui->LnEdtSeuNumero->text().toInt());
    mod->setclientid(ui->CmbBxclientid->model()->data(ui->CmbBxclientid->model()->index(ui->CmbBxclientid->currentIndex(), 0)).toInt());

    mod->setVencto(ui->DtEdtVencto->date());
    mod->setValor(ui->DblSpnBxValor->value());
    mod->setPagoEm(ui->DtEdtPagoEm->date());
    mod->setValorPago(ui->DblSpnBxValorPago->value());
    mod->setidticket(ui->CmbBxidticket->model()->data(ui->CmbBxidticket->model()->index(ui->CmbBxidticket->currentIndex(), 0)).toInt());

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

void Editticket::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtNossoNumero->setText(QString("%1").arg(m_mod->getNossoNumero()));
    ui->LnEdtSeuNumero->setText(QString("%1").arg(m_mod->getSeuNumero()));
    ui->CmbBxclientid->setCurrentId(m_mod->getclientid());
    ui->DtEdtVencto->setDate(m_mod->getVencto());
    ui->DblSpnBxValor->setValue(m_mod->getValor());
    ui->DtEdtPagoEm->setDate(m_mod->getPagoEm());
    ui->DblSpnBxValorPago->setValue(m_mod->getValorPago());
    ui->CmbBxidticket->setCurrentId(m_mod->getidticket());

}

void Editticket::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
ticket* Editticket::GetSaved()
{
   return m_lastMod;

}

