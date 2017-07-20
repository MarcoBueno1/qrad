#include "editfiles.h"
#include "ui_editfiles.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QFileDialog>
#include <QDesktopServices>


Editfiles::Editfiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editfiles)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    ui->CmbBxTipo->setTable("filedescription.Tipo");
    ui->CmbBxTipo->setField("description.Descrição");
    ui->CmbBxTipo->setCanAdd(true);
    ui->CmbBxTipo->setUserName("dsm");
    ui->CmbBxTipo->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->pushButtonSelecionar, SIGNAL(clicked()),this,SLOT(doSelect()));
    connect(ui->PshButtonVisualizar, SIGNAL(clicked()),this,SLOT(doView()));
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    ui->lineEditName->setEnabled(false);
    ui->PshButtonVisualizar->setEnabled(false);
}

Editfiles::~Editfiles()
{
    delete ui;
}
void Editfiles::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editfiles::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editfiles::SetModel(Files* mod)
{
   m_mod = mod;
   Load();
}


void Editfiles::Save()
{
    if(ui->lineEditName->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Atenção","Por favor, selecione o arquivo a ser salvo");
        ui->pushButtonSelecionar->setFocus();
        return;
    }
    if(ui->LnEdtDescricao->text().trimmed().isEmpty())
    {
        QMessageBox::warning(this, "Atenção","Por favor, Descreva este arquivo");
        ui->LnEdtDescricao->setFocus();
        return;
    }

    Files* mod =  m_mod;
    if( m_mod == NULL)
    {
        mod = new Files;
        mod->setCreated(QDate::currentDate());
    }

    mod->setName(ui->lineEditName->text());
    mod->setDescription(ui->LnEdtDescricao->text());
    mod->setLastAccess(QDate::currentDate());
    mod->setTypeId(ui->CmbBxTipo->model()->data(ui->CmbBxTipo->model()->index(ui->CmbBxTipo->currentIndex(), 0)).toInt());
    int nLObject =  mod->saveFile(ui->lineEditName->text());
    if( !nLObject )
    {
        QMessageBox::warning(this, "Atenção","Erro ao salvar o arquivo no banco de dados, por favor verifique se este arquivo existe");
        ui->LnEdtDescricao->setFocus();
        return;
    }
    mod->setLoId(nLObject);

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

void Editfiles::Load()
{
    if( m_mod == NULL)
      return;
//    ui->DtEdtCriadoEm->setDate(m_mod->getCreated());
    ui->lineEditName->setText(m_mod->getName());
    ui->LnEdtDescricao->setText(m_mod->getDescription());
//    ui->DtEdtUltimoAcesso->setDate(m_mod->getLastAccess());
    ui->CmbBxTipo->setCurrentId(m_mod->getTypeId()  );

    ui->PshButtonVisualizar->setEnabled(true);


}

void Editfiles::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Files* Editfiles::GetSaved()
{
   return m_lastMod;

}


void Editfiles::doSelect()
{
    QString Path = QFileDialog::getOpenFileName(this,
        "Selecionar Arquivo", "", "*.*");

    if(!Path.isEmpty())
    {
        ui->lineEditName->setText(Path);
        ui->PshButtonVisualizar->setEnabled(true);
    }
}
void Editfiles::doView()
{
    QDesktopServices::openUrl(QUrl::fromLocalFile(QString("%1").arg(ui->lineEditName->text())));//, QUrl::TolerantMode);
    if(m_mod)
    {
        m_mod->updateLastAccess(QDate::currentDate());
    }

}
