#include "editticketconfig.h"
#include "ui_editticketconfig.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>


Editticketconfig::Editticketconfig(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editticketconfig)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
        ui->CmbBxRespEmis->setTable("respemis");
    ui->CmbBxRespEmis->setField("Description");
    ui->CmbBxRespEmis->setCanAdd(true);
    ui->CmbBxRespEmis->setUserName("QRad");
    ui->CmbBxRespEmis->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxTipoPessoa->setTable("tipopessoa");
    ui->CmbBxTipoPessoa->setField("Description");
    ui->CmbBxTipoPessoa->setCanAdd(true);
    ui->CmbBxTipoPessoa->setUserName("QRad");
    ui->CmbBxTipoPessoa->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxLayoutBol->setTable("layoutbol");
    ui->CmbBxLayoutBol->setField("Description");
    ui->CmbBxLayoutBol->setCanAdd(true);
    ui->CmbBxLayoutBol->setUserName("QRad");
    ui->CmbBxLayoutBol->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxTipoCobranca->setTable("tipocobranca");
    ui->CmbBxTipoCobranca->setField("Description");
    ui->CmbBxTipoCobranca->setCanAdd(true);
    ui->CmbBxTipoCobranca->setUserName("QRad");
    ui->CmbBxTipoCobranca->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxcnab->setTable("cnab");
    ui->CmbBxcnab->setField("Description");
    ui->CmbBxcnab->setCanAdd(true);
    ui->CmbBxcnab->setUserName("QRad");
    ui->CmbBxcnab->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
}

Editticketconfig::~Editticketconfig()
{
    delete ui;
}
void Editticketconfig::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editticketconfig::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editticketconfig::SetModel(ticketconfig* mod)
{
   m_mod = mod;
   Load();
}


void Editticketconfig::Save()
{

    ticketconfig* mod =  m_mod;
    if( m_mod == NULL)
        mod = new ticketconfig;

    mod->setRespEmis(ui->CmbBxRespEmis->model()->data(ui->CmbBxRespEmis->model()->index(ui->CmbBxRespEmis->currentIndex(), 0)).toInt());

    mod->setTipoPessoa(ui->CmbBxTipoPessoa->model()->data(ui->CmbBxTipoPessoa->model()->index(ui->CmbBxTipoPessoa->currentIndex(), 0)).toInt());

    mod->setCodigoCedente(ui->LnEdtCodigoCedente->text());
    mod->setLayoutBol(ui->CmbBxLayoutBol->model()->data(ui->CmbBxLayoutBol->model()->index(ui->CmbBxLayoutBol->currentIndex(), 0)).toInt());

    mod->setTipoCobranca(ui->CmbBxTipoCobranca->model()->data(ui->CmbBxTipoCobranca->model()->index(ui->CmbBxTipoCobranca->currentIndex(), 0)).toInt());

    mod->setcnab(ui->CmbBxcnab->model()->data(ui->CmbBxcnab->model()->index(ui->CmbBxcnab->currentIndex(), 0)).toInt());

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

void Editticketconfig::Load()
{
    if( m_mod == NULL)
      return;
    ui->CmbBxRespEmis->setCurrentId(m_mod->getRespEmis());
    ui->CmbBxTipoPessoa->setCurrentId(m_mod->getTipoPessoa());
    ui->LnEdtCodigoCedente->setText(m_mod->getCodigoCedente());
    ui->CmbBxLayoutBol->setCurrentId(m_mod->getLayoutBol());
    ui->CmbBxTipoCobranca->setCurrentId(m_mod->getTipoCobranca());
    ui->CmbBxcnab->setCurrentId(m_mod->getcnab());

}

void Editticketconfig::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
ticketconfig* Editticketconfig::GetSaved()
{
   return m_lastMod;

}

