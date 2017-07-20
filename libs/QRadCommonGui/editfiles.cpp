#include "editfiles.h"
#include "ui_editfiles.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QFileDialog>
#include <QUrl>
#include <QDesktopServices>
#include "qradprogresswindow.h"
//#include "managerassociation.h"


Editfiles::Editfiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editfiles)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    m_associatedId =0;
    ui->CmbBxTipo->setTable("filedescription.Tipo");
    ui->CmbBxTipo->setField("description.Descrição");
    ui->CmbBxTipo->setCanAdd(true);
    ui->CmbBxTipo->setUserName("dsm");
    ui->CmbBxTipo->completer()->setFilterMode(Qt::MatchContains );


    ui->comboBoxAssociar->setTable("fileassociation.Associado A");
    ui->comboBoxAssociar->setField("description.Descrição");
    ui->comboBoxAssociar->setCanAdd(false);
    ui->comboBoxAssociar->setUserName("dsm");
    ui->comboBoxAssociar->completer()->setFilterMode(Qt::MatchContains );

    ui->comboBoxAssociar->setVisible(false);
    ui->labelAssociar->setVisible(false);

    connect(ui->comboBoxAssociar, SIGNAL(activated(int)), this, SLOT(doAssociate(int)));
    connect(ui->pushButtonSelecionar, SIGNAL(clicked()),this,SLOT(doSelect()));
    connect(ui->PshButtonVisualizar, SIGNAL(clicked()),this,SLOT(doView()));
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    ui->lineEditName->setEnabled(false);
    ui->PshButtonVisualizar->setEnabled(false);

    ui->dateTimeEditCreated->setDate(QDate::currentDate());
    ui->dateTimeEditLastAccesss->setDate(QDate::currentDate());
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

    QRAD_SHOW_PRPGRESS("Preparando informações...")

    Files* mod =  m_mod;
    if( m_mod == NULL)
    {
        mod = new Files;
        mod->setCreated(QDate::currentDate());
    }

    mod->setDescription(ui->LnEdtDescricao->text());
    mod->setLastAccess(QDate::currentDate());
    mod->setTypeId(ui->CmbBxTipo->model()->data(ui->CmbBxTipo->model()->index(ui->CmbBxTipo->currentIndex(), 0)).toInt());
    mod->setAssociationTypeId(ui->comboBoxAssociar->model()->data(ui->comboBoxAssociar->model()->index(ui->comboBoxAssociar->currentIndex(), 0)).toInt());

    QRAD_SHOW_PRPGRESS_STEP("Enviando arquivo para o servidor...");

    int nLObject =  mod->saveFile(ui->lineEditName->text());
    if( !nLObject )
    {
        QRAD_HIDE_PRPGRESS();
        QMessageBox::warning(this, "Atenção","Erro ao salvar o arquivo no banco de dados, por favor verifique se este arquivo existe");
        ui->LnEdtDescricao->setFocus();
        return;
    }
    mod->setLoId(nLObject);
    QString filename = ui->lineEditName->text();
    filename = filename.mid(filename.lastIndexOf("/")+1);

    mod->setName(filename);

    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;
    QRAD_HIDE_PRPGRESS();
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
    ui->dateTimeEditCreated->setDate(m_mod->getCreated());
    ui->dateTimeEditLastAccesss->setDate(m_mod->getLastAccess());

    ui->lineEditName->setText(m_mod->getName());
    ui->LnEdtDescricao->setText(m_mod->getDescription());

    ui->CmbBxTipo->setCurrentId(m_mod->getTypeId()  );
    ui->comboBoxAssociar->setCurrentId(m_mod->getAssociationTypeId());

    ui->PshButtonVisualizar->setEnabled(true);
    m_associatedId = m_mod->getAssociationId();
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
        ui->LnEdtDescricao->selectAll();
        ui->LnEdtDescricao->setFocus();
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
void Editfiles::setFile(QString path)
{
    ui->lineEditName->setText(path);
    ui->LnEdtDescricao->setFocus();
}



void Editfiles::doAssociate(int current)
{
/*    QString sql;
    Managerassociation *ma = new Managerassociation;
    switch( (QRAD_ASSOCIATION_TYPE)current )
    {
        case tpAccountToReceive:
        case tpAccountToPay:
             sql = "select id, issuedate as \"Criada Em\", vencdate as \"Vencto\", description as \"Descrição\" from fin_accounttoreceive order by id desc limit 5000 ";
             ma->setSQL(sql,(QRAD_ASSOCIATION_TYPE)current);
             if( (ma->exec() == QDialog::Accepted) && ma->getSelectedId())
             {
                 m_associatedId = ma->getSelectedId();
             }
             break;
        case tpdweller:
        break;
        case tpsupplier:
        break;
        case tpuser:
        break;
        case tpNone:
        default:
             m_associatedId =0;
        break;
    }

  delete ma;
  */
}
