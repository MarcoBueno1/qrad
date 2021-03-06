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
    ui->CmbBxRespEmis->setCanAdd(false);
    ui->CmbBxRespEmis->setUserName("dsm");
    ui->CmbBxRespEmis->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxTipoPessoa->setTable("tipopessoa");
    ui->CmbBxTipoPessoa->setField("Description");
    ui->CmbBxTipoPessoa->setCanAdd(false);
    ui->CmbBxTipoPessoa->setUserName("dsm");
    ui->CmbBxTipoPessoa->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxLayoutBol->setTable("layoutbol");
    ui->CmbBxLayoutBol->setField("Description");
    ui->CmbBxLayoutBol->setCanAdd(false);
    ui->CmbBxLayoutBol->setUserName("dsm");
    ui->CmbBxLayoutBol->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxTipoCobranca->setTable("tipocobranca");
    ui->CmbBxTipoCobranca->setField("Description");
    ui->CmbBxTipoCobranca->setCanAdd(false);
    ui->CmbBxTipoCobranca->setUserName("dsm");
    ui->CmbBxTipoCobranca->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxcnab->setTable("cnab");
    ui->CmbBxcnab->setField("Description");
    ui->CmbBxcnab->setCanAdd(false);
    ui->CmbBxcnab->setUserName("dsm");
    ui->CmbBxcnab->completer()->setFilterMode(Qt::MatchContains );
    ui->CmbBxespecie->setTable("especie");
    ui->CmbBxespecie->setField("Description");
    ui->CmbBxespecie->setCanAdd(false);
    ui->CmbBxespecie->setUserName("dsm");
    ui->CmbBxespecie->completer()->setFilterMode(Qt::MatchContains );

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

    mod->setespecie(ui->CmbBxespecie->model()->data(ui->CmbBxespecie->model()->index(ui->CmbBxespecie->currentIndex(), 0)).toInt());

    mod->setNossoNumero(ui->DblSpnBxNossoNumero->value());
    mod->setCarteira(ui->LnEdtCarteira->text());
    mod->setMensagem(ui->textEditMensagem->document()->toPlainText());
    mod->setJuros(QString("%1").arg(ui->doubleSpinBoxJuros->value()).replace(".",","));
    mod->setMulta(QString("%1").arg(ui->doubleSpinBoxMulta->value()).replace(".",","));
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
    ui->CmbBxespecie->setCurrentId(m_mod->getespecie());
    ui->DblSpnBxNossoNumero->setValue(m_mod->getNossoNumero());
    ui->LnEdtCarteira->setText(m_mod->getCarteira());
    ui->textEditMensagem->setText(m_mod->getMensagem());
    ui->doubleSpinBoxJuros->setValue(m_mod->getJuros().replace(",",".").toDouble());
    ui->doubleSpinBoxMulta->setValue(m_mod->getMulta().replace(",",".").toDouble());
    ui->LnEdtDescription->setText(m_mod->getDescription());

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

