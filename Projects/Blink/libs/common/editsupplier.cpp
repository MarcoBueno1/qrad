#include "editsupplier.h"
#include "ui_editsupplier.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include "editphone.h"
#include "address.h"
#include "addressv2.h"
#include "editaddress.h"
#include "column2delegate.h"
#include <QNetworkRequest>
#include <QUrl>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>



Editsupplier::Editsupplier(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editsupplier)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonAddPhone, SIGNAL(clicked()),this,SLOT(AddPhone()));
    connect(ui->pushButtonRemovePhone, SIGNAL(clicked()),this,SLOT(RemovePhone()));

    connect(ui->pushButtonAddAddress, SIGNAL(clicked()),this,SLOT(AddAddress()));
    connect(ui->pushButtonRemoveAddress, SIGNAL(clicked()),this,SLOT(RemoveAddress()));
    connect(ui->LnEdtCNPJ,SIGNAL(textEdited(QString)),this,SLOT(onCNPJEdited(QString)));


    m_mod = new supplier;
    m_mod->setRemoved(true);
    m_mod->Save();

    m_model = new QSqlQueryModel;
    m_AddressModel= new QSqlQueryModel;
    m_PhoneDelegate = new ColumnPhone;
    m_CenterDelegate = new ColumnCenter;
    m_BooleanDelegate= new ColumnBool;

    m_manager = new QNetworkAccessManager;
    connect(m_manager, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(replyFinished(QNetworkReply*)));



    RefreshPhoneTable();

}

Editsupplier::~Editsupplier()
{
    delete m_model;
    delete m_AddressModel;
    delete m_PhoneDelegate;
    delete m_CenterDelegate;   
    delete m_BooleanDelegate;
    delete m_manager;

    if(  m_mod )
       delete m_mod;

    delete ui;

}
void Editsupplier::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editsupplier::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editsupplier::SetModel(supplier* mod)
{
   m_mod = mod;
   Load();
}


void Editsupplier::Save()
{

    supplier* mod =  m_mod;
    if( m_mod == NULL)
        mod = new supplier;

    mod->setCNPJ(ui->LnEdtCNPJ->text());
    mod->setNome(ui->LnEdtNome->text());
    mod->setFantasia(ui->LnEdtFantasia->text());
    mod->setData(QDate::currentDate());
    mod->setHora(QTime::currentTime());
    mod->setRemoved(false);
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

void Editsupplier::Load()
{
    if( m_mod == NULL)
      return;
    ui->LnEdtCNPJ->setText(m_mod->getCNPJ());
    ui->LnEdtNome->setText(m_mod->getNome());
    ui->LnEdtFantasia->setText(m_mod->getFantasia());
    RefreshPhoneTable();
}

void Editsupplier::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
supplier* Editsupplier::GetSaved()
{
   return m_lastMod;

}

void Editsupplier::AddPhone()
{
    Editphone *phone = new Editphone();

    phone->setOwner(m_mod->getid());
    phone->setOwnerType(tpSupplier);
    if(phone->exec() == QDialog::Accepted )
    {
       RefreshPhoneTable();
    }
    delete phone;
}

void Editsupplier::RemovePhone()
{
    int id = ui->tableViewPhone->model()->index(ui->tableViewPhone->currentIndex().row(),0).data().toInt();
    Phone *p = Phone::findByPrimaryKey(id);
    if(p)
    {
        p->updateRemoved( true );
        delete p;
        RefreshPhoneTable();
    }
}

void Editsupplier::RefreshPhoneTable()
{
   // qrad -s phone -t phone -c id -i int -c dwellerid -i int -c number -i QString -c operator -i int:multi:Operadora.Name[Tim,Oi,Claro,Vivo] -c type -i int:multi:phonetype.type[Celular,Casa,Trabalho] -c watsapp -i bool



  QString SQL = QString("select p.id, p.Number as \"Numero\", o.name as \"Operadora\", t.type as \"Tipo\", watsapp as \"WatsApp\" "\
                        "from phone p inner join operadora o on o.id = p.operator  "\
                        "inner join phonetype t on t.id = p.type  "\
                        "where p.owner = %1 and ownertype = 2 ").arg(m_mod?m_mod->getid():0);

  m_model->setQuery(SQL);
  ui->tableViewPhone->setModel(m_model);
  ui->tableViewPhone->hideColumn(0);
  ui->tableViewPhone->setItemDelegateForColumn(1, m_PhoneDelegate);
  ui->tableViewPhone->setItemDelegateForColumn(2, m_CenterDelegate);
  ui->tableViewPhone->setItemDelegateForColumn(3, m_CenterDelegate);
  ui->tableViewPhone->setItemDelegateForColumn(4, m_BooleanDelegate);

  ui->tableViewPhone->resizeColumnsToContents();
  ui->tableViewPhone->resizeRowsToContents();
 // ui->tableViewPhone->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
 // ui->tableViewPhone->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  ui->tableViewPhone->horizontalHeader()->setStretchLastSection(true);

}

void Editsupplier::AddAddress()
{
    Editaddress *pAddress = new Editaddress();
    pAddress->setOwner(m_mod->getid());
    pAddress->setOwnerType(tpSupplier);
    if(pAddress->exec() == QDialog::Accepted )
    {
       RefreshAddressTable();
    }
    delete pAddress;
}

void Editsupplier::RemoveAddress()
{
    int id = ui->tableViewAddress->model()->index(ui->tableViewAddress->currentIndex().row(),0).data().toInt();
    Address *p = Address::findByid(id);
    if(p)
    {
        p->updateRemoved( true );
        delete p;
        RefreshAddressTable();
    }
}

void Editsupplier::RefreshAddressTable()
{
   // qrad -s phone -t phone -c id -i int -c dwellerid -i int -c number -i QString -c operator -i int:multi:Operadora.Name[Tim,Oi,Claro,Vivo] -c type -i int:multi:phonetype.type[Celular,Casa,Trabalho] -c watsapp -i bool


    QString SQL = QString("select a.id, s.name as \"Rua\", a.number as \"Número\", c.Number as \"Cep\", n.name as \"Bairro\", ci.name as \"Cidade\", st.name as \"Estado\" "\
                          " from address a "\
                          " inner join street s on s.id = a.street"\
                          " inner join cep c on c.id = a.cep"\
                          " inner join neighborhood n on n.id = a.neighborhood"\
                          " inner join city ci on ci.id = a.city"\
                          " inner join state st on st.id = a.state"\
                          " where ownertype = 2 and owner = %1").arg(m_mod->getid());

  m_AddressModel->setQuery(SQL);
  ui->tableViewAddress->setModel(m_AddressModel);
  ui->tableViewAddress->hideColumn(0);
  ui->tableViewAddress->setItemDelegateForColumn(1, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(2, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(3, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(4, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(5, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(6, m_CenterDelegate);

//  ui->tableViewAddress->resizeColumnsToContents();
//  ui->tableViewAddress->resizeRowsToContents();
//  ui->tableViewAddress->horizontalHeader()->setStretchLastSection(true);
  ui->tableViewAddress->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
  ui->tableViewAddress->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

void Editsupplier::onCNPJEdited(QString cnpj)
{
  debug_message("cnpj=%s\n",cnpj.toLatin1().data());
  QString local_cnpj = cnpj.trimmed().remove(".").remove("-").remove("/");
  if(local_cnpj.length() == 14)
  {
      QString req = QString("https://www.receitaws.com.br/v1/cnpj/%1").arg(local_cnpj);
      m_manager->get(QNetworkRequest(QUrl(req)));
      debug_message("tentando acessar %s\n",req.toLatin1().data());

  }
}
void Editsupplier::replyFinished(QNetworkReply* pReply)
{

    QByteArray data=pReply->readAll();
    QString str(data);

    debug_message("resposta recebida\n%s\n",str.toLatin1().data());

    QJsonParseError err;
    QJsonDocument doc = QJsonDocument::fromJson(data, &err);

    QJsonObject obj = doc.object();

    QJsonValue val = obj.value("nome");
    ui->LnEdtNome->setText(val.toString());
    val = obj.value("fantasia");
    ui->LnEdtFantasia->setText( val.toString());

    val = obj.value("cep");
//    AddressV2 *pAddress = AddressV2::findBycep(val.toString().remove(".").remove("-"));
//    if( !pAddress )
    {
        /*
        pAddress = new AddressV2;
        pAddress->setcep(val.toString().remove(".").remove("-"));
        val = obj.value("cep");
        pAddress->setcity();
        */
        //fazer amanha
    }

    //process str any way you like!

}
