#include "editdweller.h"
#include "ui_editdweller.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include "qraddebug.h"
#include "editphone.h"
#include "docvalidate.h"
#include "camera.h"
#include "editaddress.h"

#define PORTEIRO_FUL_PATH "dweller.jpg"

EditDweller::EditDweller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDweller)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonAddPhone, SIGNAL(clicked()),this,SLOT(AddPhone()));
    connect(ui->pushButtonRemovePhone, SIGNAL(clicked()),this,SLOT(RemovePhone()));
    connect(ui->pushButtonBaterFoto, SIGNAL(clicked()),this, SLOT(baterFoto()));

    connect(ui->pushButtonAddAddress, SIGNAL(clicked()),this,SLOT(AddAddress()));
    connect(ui->pushButtonRemoveAddress, SIGNAL(clicked()),this,SLOT(RemoveAddress()));

    debug_message("-->EditDweller\n");

    ui->comboBoxAp->setTable("ap");
    ui->comboBoxAp->setField("Numero");
    ui->comboBoxAp->setCanAdd(true);
    ui->comboBoxAp->setUserName("dsm");
    if( ui->comboBoxAp->completer() )
        ui->comboBoxAp->completer()->setFilterMode(Qt::MatchContains );

    debug_message("---\n");

    ui->comboBoxApTorre->setTable("tower");
    ui->comboBoxApTorre->setField("Name");
    ui->comboBoxApTorre->setCanAdd(true);
    ui->comboBoxApTorre->setUserName("dsm");
    if( ui->comboBoxApTorre->completer() )
        ui->comboBoxApTorre->completer()->setFilterMode(Qt::MatchContains );
	
    ui->comboBoxTipo->setTable("dweller_type");
    ui->comboBoxTipo->setField("Name");
    ui->comboBoxTipo->setCanAdd(true);
    ui->comboBoxTipo->setUserName("dsm");
    if( ui->comboBoxTipo->completer() )
        ui->comboBoxTipo->completer()->setFilterMode(Qt::MatchContains );

	ui->comboBoxProfissao->setTable("jobtitle");
    ui->comboBoxProfissao->setField("Name");
    ui->comboBoxProfissao->setCanAdd(true);
    ui->comboBoxProfissao->setUserName("dsm");
    if( ui->comboBoxProfissao->completer() )
        ui->comboBoxProfissao->completer()->setFilterMode(Qt::MatchContains );

    m_model = new QSqlQueryModel;
    m_AddressModel = new QSqlQueryModel;
    m_PhoneDelegate = new ColumnPhone;
    m_CenterDelegate = new ColumnCenter;
    m_BooleanDelegate= new ColumnBool;
 
    m_mod = new Dweller;
    m_mod->setRemoved(true);
    m_mod->Save();
}

EditDweller::~EditDweller()
{
    delete ui;
    delete m_model ;
    delete m_AddressModel;
    delete m_PhoneDelegate;
    delete m_CenterDelegate;
    delete m_BooleanDelegate;
    if(  m_mod )
       delete m_mod;
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
   delete m_mod;

   m_mod = mod;
   Load();
}



void EditDweller::Save()
{
    QString strNome = ui->lineEditDweller->text().trimmed();
    if(strNome.isEmpty())
    {
        QMessageBox::warning(this,
                                 "Atenção",
                                 "Nome do morador não pode ficar vazio");
        ui->lineEditDweller->selectAll();
        ui->lineEditDweller->setFocus();
        return;
    }

    QString cpf = ui->lineEditCPF->text();

    cpf = cpf.remove("-").remove(".").trimmed();

    if( cpf.isEmpty() )
    {
        if( QMessageBox::No == QMessageBox::question(this,
                                  "Atenção",
                                  "Campo CPF está vazio, deseja prosseguir mesmo assim?",
                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
        {
            ui->lineEditCPF->selectAll();
            ui->lineEditCPF->setFocus();
            return;
        }
    }
    else if( !CCPF::IsValid(cpf))
    {
        QMessageBox::warning(this,
                                 "Atenção",
                                 "Conteúdo do campo CPF inválido, preencha-o ou deixe-o vazio");
        ui->lineEditCPF->selectAll();
        ui->lineEditCPF->setFocus();
        return;
    }
    if( !cpf.isEmpty() )
    {
      // verifica se ja foi cadastrado previamente

      Dweller *d = Dweller::findByCPF( cpf );
      if( d )
      {
        QMessageBox::warning(this,
                             "Oops!",
                              QString("Já existe um morador cadastrado com este CPF (%1 ) ").arg(d->getName()));
        ui->lineEditCPF->selectAll();
        ui->lineEditCPF->setFocus();
        delete d;
        return;
      }
    }
   
    m_mod->setName(ui->lineEditDweller->text());
    m_mod->setCPF(ui->lineEditCPF->text());
    m_mod->setRG(ui->lineEditRG->text());

    m_mod->setAp( ui->comboBoxAp->model()->data(ui->comboBoxAp->model()->index(ui->comboBoxAp->currentIndex(), 0)).toInt());
    m_mod->setTower( ui->comboBoxApTorre->model()->data(ui->comboBoxApTorre->model()->index(ui->comboBoxApTorre->currentIndex(), 0)).toInt() );
    m_mod->setType( ui->comboBoxTipo->model()->data(ui->comboBoxTipo->model()->index(ui->comboBoxTipo->currentIndex(), 0)).toInt() );
    m_mod->setJobTitle( ui->comboBoxProfissao->model()->data(ui->comboBoxProfissao->model()->index(ui->comboBoxProfissao->currentIndex(), 0)).toInt() ) ;
    m_mod->setemail( ui->lineEditEmail->text());
    m_mod->setRamal(ui->lineEditRamal->text());
    m_mod->setSince(ui->dateEditSince->date());
    m_mod->setMoveOut(ui->dateEditUntil->date());
    m_mod->setObs(ui->textEditObs->toPlainText());
    m_mod->setRemoved(false);


    bool bRet = m_mod->Save();
    if( bRet )
    {
       m_mod->saveImage( PORTEIRO_FUL_PATH );
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
    ui->lineEditDweller->setText(m_mod->getName());
    ui->lineEditCPF->setText(m_mod->getCPF());
    ui->lineEditRG->setText(m_mod->getRG());
    ui->lineEditEmail->setText(m_mod->getemail());
    ui->lineEditRamal->setText(m_mod->getRamal());
    ui->dateEditSince->setDate(m_mod->getSince());
    ui->dateEditUntil->setDate(m_mod->getMoveOut());
    ui->textEditObs->setText(m_mod->getObs());

    ui->comboBoxAp->setCurrentId(m_mod->getAp());
    ui->comboBoxApTorre->setCurrentId(m_mod->getTower());
    ui->comboBoxTipo->setCurrentId(m_mod->getType());
    ui->comboBoxProfissao->setCurrentId(m_mod->getJobTitle());

    RefreshAddressTable();
    RefreshPhoneTable();
//    RefreshVeicTable();

    QPixmap mypix = m_mod->getImage();
    ui->LblPhoto->setPixmap(mypix);
}

void EditDweller::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Dweller* EditDweller::GetSaved()
{
   return m_mod;

}
void EditDweller::AddPhone()
{
    Editphone *phone = new Editphone();

    phone->setOwner(m_mod->getid());
    phone->setOwnerType(tpDweller);
    if(phone->exec() == QDialog::Accepted )
    {
       RefreshPhoneTable();
    }
    delete phone;
}
void EditDweller::AddAddress()
{
    Editaddress *pAddress = new Editaddress();
    pAddress->setOwner(m_mod->getid());
    pAddress->setOwnerType(tpDweller);
    if(pAddress->exec() == QDialog::Accepted )
    {
       RefreshAddressTable();
    }
    delete pAddress;
}
void EditDweller::RemovePhone()
{
    int id = ui->tableViewPhone->model()->index(ui->tableViewPhone->currentIndex().row(),0).data().toInt();
    Phone *p = Phone::findByid(id);
    if(p)
    {
        p->updateRemoved( true );
        delete p;
        RefreshPhoneTable();
    }
}
void EditDweller::RemoveAddress()
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

void EditDweller::RefreshPhoneTable()
{
   // qrad -s phone -t phone -c id -i int -c dwellerid -i int -c number -i QString -c operator -i int:multi:Operadora.Name[Tim,Oi,Claro,Vivo] -c type -i int:multi:phonetype.type[Celular,Casa,Trabalho] -c watsapp -i bool



  QString SQL = QString("select p.id, p.Number as \"Numero\", o.name as \"Operadora\", t.type as \"Tipo\", watsapp as \"WatsApp\" "\
                        "from phone p inner join operadora o on o.id = p.operator  "\
                        "inner join phonetype t on t.id = p.type  "\
                        "where p.owner = %1 and ownertype = 0 ").arg(m_mod?m_mod->getid():0);

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
void EditDweller::RefreshAddressTable()
{
   // qrad -s phone -t phone -c id -i int -c dwellerid -i int -c number -i QString -c operator -i int:multi:Operadora.Name[Tim,Oi,Claro,Vivo] -c type -i int:multi:phonetype.type[Celular,Casa,Trabalho] -c watsapp -i bool


    QString SQL = QString("select a.id, s.name as \"Rua\", a.number as \"Número\", c.Number as \"Cep\", n.name as \"Bairro\", ci.name as \"Cidade\", st.name as \"Estado\" "\
                          " from address a "\
                          " inner join street s on s.id = a.street"\
                          " inner join cep c on c.id = a.cep"\
                          " inner join neighborhood n on n.id = a.neighborhood"\
                          " inner join city ci on ci.id = a.city"\
                          " inner join state st on st.id = a.state"\
                          " where ownertype = 0 and owner = %1").arg(m_mod->getid());

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
/*
void EditDweller::RefreshVeicTable()
{
   // qrad -s phone -t phone -c id -i int -c dwellerid -i int -c number -i QString -c operator -i int:multi:Operadora.Name[Tim,Oi,Claro,Vivo] -c type -i int:multi:phonetype.type[Celular,Casa,Trabalho] -c watsapp -i bool



  QString SQL = QString("select dv.id, dv.board as \"Placa\", m.name as \"Marca\", v.name as \"Nome\" "\
                        "from dweller_veic dv inner join veic_brand m on m.id = dv.brand  "\
                        "inner join veic_name v on dv.name = v.id  "\
                        "where dv.owner = %1 and ownertype = 0 ").arg(m_mod?m_mod->getid():0);

  m_model->setQuery(SQL);
  ui->tableViewVeiculos->setModel(m_model);
  ui->tableViewVeiculos->hideColumn(0);
  ui->tableViewVeiculos->setItemDelegateForColumn(1, m_PhoneDelegate);
  ui->tableViewVeiculos->setItemDelegateForColumn(2, m_CenterDelegate);
  ui->tableViewVeiculos->setItemDelegateForColumn(3, m_CenterDelegate);
  ui->tableViewVeiculos->setItemDelegateForColumn(4, m_BooleanDelegate);

  ui->tableViewVeiculos->resizeColumnsToContents();
  ui->tableViewVeiculos->resizeRowsToContents();
 // ui->tableViewPhone->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
 // ui->tableViewPhone->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

  ui->tableViewVeiculos->horizontalHeader()->setStretchLastSection(true);

}
*/

void EditDweller::baterFoto()
{
  Camera *cam = new Camera;

  cam->setPath(PORTEIRO_FUL_PATH);
  if( QDialog::Accepted == cam->exec())
  {
      QImage myImage ;
      myImage.load(PORTEIRO_FUL_PATH);
      ui->LblPhoto->setPixmap(QPixmap::fromImage(myImage));
      ui->PshBtnSave->setFocus(Qt::TabFocusReason);
  }
  delete cam;
}
