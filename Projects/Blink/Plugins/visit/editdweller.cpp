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

    ui->comboBoxAp->setCurrentId(m_mod->getAp());
    ui->comboBoxApTorre->setCurrentId(m_mod->getTower());
    ui->comboBoxTipo->setCurrentId(m_mod->getType());
    ui->comboBoxProfissao->setCurrentId(m_mod->getJobTitle());

    RefreshAddressTable();
    RefreshPhoneTable();

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
    if(phone->exec() == QDialog::Accepted )
    {
       RefreshPhoneTable();
    }
    delete phone;
}
void EditDweller::AddAddress()
{
    Editaddress *pAddress = new Editaddress();
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
                        "where p.dwellerid = %1").arg(m_mod?m_mod->getid():0);

  m_model->setQuery(SQL);
  ui->tableViewPhone->setModel(m_model);
  ui->tableViewPhone->hideColumn(0);
  ui->tableViewPhone->setItemDelegateForColumn(1, m_PhoneDelegate);
  ui->tableViewPhone->setItemDelegateForColumn(2, m_CenterDelegate);
  ui->tableViewPhone->setItemDelegateForColumn(3, m_CenterDelegate);
  ui->tableViewPhone->setItemDelegateForColumn(4, m_BooleanDelegate);

  ui->tableViewPhone->resizeColumnsToContents();
  ui->tableViewPhone->resizeRowsToContents();
  ui->tableViewPhone->horizontalHeader()->setStretchLastSection(true);


}
void EditDweller::RefreshAddressTable()
{
   // qrad -s phone -t phone -c id -i int -c dwellerid -i int -c number -i QString -c operator -i int:multi:Operadora.Name[Tim,Oi,Claro,Vivo] -c type -i int:multi:phonetype.type[Celular,Casa,Trabalho] -c watsapp -i bool


  QString SQL = QString("select p.id, p.Number as \"Numero\", o.name as \"Operadora\", t.type as \"Tipo\", watsapp as \"WatsApp\" "\
                        "from phone p inner join operadora o on o.id = p.operator  "\
                        "inner join phonetype t on t.id = p.type  "\
                        "where p.dwellerid = %1").arg(m_mod?m_mod->getid():0);

  m_AddressModel->setQuery(SQL);
  ui->tableViewAddress->setModel(m_AddressModel);
  ui->tableViewAddress->hideColumn(0);
  ui->tableViewAddress->setItemDelegateForColumn(1, m_PhoneDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(2, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(3, m_CenterDelegate);
  ui->tableViewAddress->setItemDelegateForColumn(4, m_BooleanDelegate);

  ui->tableViewAddress->resizeColumnsToContents();
  ui->tableViewAddress->resizeRowsToContents();
  ui->tableViewAddress->horizontalHeader()->setStretchLastSection(true);
}

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
