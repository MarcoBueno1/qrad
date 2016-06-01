#include "editdweller.h"
#include "ui_editDweller.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include "qraddebug.h"
#include "editphone.h"
#include "docvalidate.h"
#include "camera.h"

#define PORTEIRO_FUL_PATH "deller.jpg"

EditDweller::EditDweller(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::EditDweller)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    
    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonAddPhone, SIGNAL(clicked()),this,SLOT(AddPhone()));
    connect(ui->pushButtonRemovePhone, SIGNAL(clicked()),this,SLOT(RemovePhone()));
    connect(ui->pushButtonBaterFoto, SIGNAL(clicked()),this, SLOT(baterFoto()));


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
    m_PhoneDelegate = new ColumnPhone;
    m_CenterDelegate = new ColumnCenter;
    m_BooleanDelegate= new ColumnBool;

}

EditDweller::~EditDweller()
{
    delete ui;
    delete m_model ;
    delete m_PhoneDelegate;
    delete m_CenterDelegate;
    delete m_BooleanDelegate;
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

    Dweller* mod =  m_mod;
    if( m_mod == NULL)
        mod = new Dweller;

   // mod->setNome(ui->LnEdtNome->text());
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

void EditDweller::Load()
{
    if( m_mod == NULL)
      return;
  //  ui->LnEdtNome->setText(m_mod->getNome());

}

void EditDweller::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
Dweller* EditDweller::GetSaved()
{
   return m_lastMod;

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
