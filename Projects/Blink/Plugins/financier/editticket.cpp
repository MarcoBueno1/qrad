#include "editticket.h"
#include "ui_editticket.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QSqlField>


Editticket::Editticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editticket)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;

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

    mod->setVencto(ui->DtEdtVencto->date());
    mod->setValor(ui->DblSpnBxValor->value());
    mod->setObs(ui->LnEdtObs->text());

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
    ui->DtEdtVencto->setDate(m_mod->getVencto());
    ui->DblSpnBxValor->setValue(m_mod->getValor());
    ui->comboBox->setCurrentIndex(m_mod->getType());
    ui->LnEdtObs->setText(m_mod->getObs());
    QString Sql = QString("select d.id, d.name, a.numero, t.name, d.email "\
             "from dweller d "\
             "inner join tower t on t.id= d.tower "\
             "inner join ap a on a.id = d.ap  where id = %1").arg(m_mod->getclientid());
    QSqlQuery *pQ = new QSqlQuery ;
    if(pQ->exec(Sql))
    {
        pQ->first();
        ui->LnEdtNome->setText(pQ->record().field(1).value().toString());
        ui->LnEdtAp->setText(pQ->record().field(2).value().toString());
        ui->LnEdtTower->setText(pQ->record().field(3).value().toString());
        ui->LnEdtemail->setText(pQ->record().field(4).value().toString());
    }
    delete pQ;

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

