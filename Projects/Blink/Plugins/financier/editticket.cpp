#include "editticket.h"
#include "ui_editticket.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QSqlField>
#include "accounttoreceivemodel.h"
#include "accounttoreceivehistorymodel.h"
#include "qradconfig.h"


Editticket::Editticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editticket)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    setWindowTitle("Edição de Boleto");
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
   setWindowTitle(QString("Edição do Boleto %1").arg(m_mod->getNossoNumero()));
}


void Editticket::Save()
{
    ticket* mod =  m_mod;
    if( m_mod == NULL)
        mod = new ticket;

    if(mod->getStatus() != stCreated)
    {
        if(( ui->DtEdtVencto->date() != mod->getVencto()) &&( mod->getValor() != ui->DblSpnBxValor->value()))
           // mod->setStatus(stDateValueModified);
            mod->setStatus(stCreated);
        else if( ui->DtEdtVencto->date() != mod->getVencto())
           // mod->setStatus(stDateModified);
            mod->setStatus(stCreated);
        else if ( mod->getValor() != ui->DblSpnBxValor->value())
           // mod->setStatus(stValueModified);
            mod->setStatus(stCreated);

    }

    //if(mod->getStatus() == stBuiltShipp)
    //    mod->setStatus(stCreated);

    mod->setVencto(ui->DtEdtVencto->date());
    mod->setValor(ui->DblSpnBxValor->value());
    mod->setObs(ui->LnEdtObs->text());
    mod->setDiscount(ui->doubleSpinBox->value());

    bool bRet = mod->Save();
    if( m_lastMod )
       delete m_lastMod;
    m_lastMod = mod;
    m_mod = NULL;
    if( bRet )
    {
       AccountToReceiveModel *account = AccountToReceiveModel::findById(mod->getAccountId(),true);
       if( account )
       {
           account->setValue(mod->getValor());
           account->setVencDate(mod->getVencto());
           if(account->Save())
           {
            AccountToReceiveHistoryModel *his = new AccountToReceiveHistoryModel;
            his->setDate(QDate::currentDate());
            his->setTime(QTime::currentTime());
            his->setTypeOperation(AccountOpEdit);
            his->setUserId(QRadConfig::GetCurrentUserId());
            his->Save();
            delete his;
           }
           delete account;
       }
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
    ui->DtEdtVencto->setDate(m_mod->getVencto());
    ui->DblSpnBxValor->setValue(m_mod->getValor());
    ui->comboBox->setCurrentIndex(m_mod->getType());
    ui->LnEdtObs->setText(m_mod->getObs());
    ui->doubleSpinBox->setValue(m_mod->getDiscount());
    QString Sql = QString("select d.id, d.name, a.numero, t.name, d.email "\
             "from dweller d "\
             "inner join tower t on t.id= d.tower "\
             "inner join ap a on a.id = d.ap  where d.id = %1").arg(m_mod->getclientid());
    QSqlQuery *pQ = new QSqlQuery ;
    if(pQ->exec(Sql))
    {
        pQ->first();
        ui->LnEdtNome->setText(pQ->record().field(1).value().toString());
        ui->LnEdtAp->setText(pQ->record().field(2).value().toString());
        ui->LnEdtTower->setText(pQ->record().field(3).value().toString());
        ui->LnEdtemail->setText(pQ->record().field(4).value().toString());
    }
    if(pQ->exec(QString("select number from phone where owertype = 0 and owner = %1")
             .arg(pQ->record().field(0).value().toInt())))
    {
        pQ->first();
        QString cat;
        for (int i=0; i < pQ->size();i++)
        {
            cat += pQ->record().field(0).value().toString() + "  ";
            pQ->next();
        }
        ui->LnEdtPhone->setText(cat);
    }

    delete pQ;
    ui->LnEdtNome->setReadOnly(true);
    ui->LnEdtAp->setReadOnly(true);
    ui->LnEdtTower->setReadOnly(true);
    ui->LnEdtemail->setReadOnly(true);
    ui->comboBox->setEnabled(false);
    ui->LnEdtNossoNumero->setReadOnly(true);
    ui->LnEdtSeuNumero->setReadOnly(true);
    ui->LnEdtPhone->setReadOnly(true);
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

