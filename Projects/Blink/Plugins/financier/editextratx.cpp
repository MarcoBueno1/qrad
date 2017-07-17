#include "editextratx.h"
#include "ui_editextratx.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QSqlField>


Editextratx::Editextratx(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editextratx)
{
    ui->setupUi(this);
    
    m_mod = NULL;
    m_lastMod = NULL;
    ui->CmbBxmotivo->setTable("reasonextratx");
    ui->CmbBxmotivo->setField("Description.Descrição");
    ui->CmbBxmotivo->setCanAdd(true);
    ui->CmbBxmotivo->setUserName("dsm");
    ui->CmbBxmotivo->completer()->setFilterMode(Qt::MatchContains );

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonAdd, SIGNAL(clicked()),this,SLOT(Add()));
    connect(ui->pushButtonRemove, SIGNAL(clicked()),this,SLOT(Remove()));
    connect(ui->checkBoxMovedOut, SIGNAL(clicked()),this,SLOT(MovedOut()));



    ui->dateEdit->setDate(QDate::currentDate().addDays(15));

    ui->lineEditMorador->setSelect("select a.numero  || ' | '  || t.name  || ' | ' || d.name, d.id, d.name, a.numero, t.name, d.ramal "\
                                   "from dweller d "\
                                   "inner join tower t on t.id= d.tower "\
                                   "inner join ap a on a.id = d.ap  where d.removed = false and d.payer = true");
//    ui->lineEditMorador->Add(ui->lineEditAP);
    //ui->lineEditMorador->Add(ui->lineEditTorre);
//    ui->lineEditMorador->Add(ui->lineEditRamal);
    m_selected = new QSqlQueryModel;
    setWindowTitle("Nova Taxa Extra");
}

Editextratx::~Editextratx()
{
    QSqlQuery removeTable;

    removeTable.exec("delete from selectedtickets");
    removeTable.exec("drop table selectedtickets");
    delete m_selected;
    delete ui;
}
void Editextratx::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editextratx::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editextratx::SetModel(extratx* mod)
{
   m_mod = mod;
   Load();
}


void Editextratx::Save()
{

    extratx* mod =  m_mod;
    if( m_mod == NULL)
        mod = new extratx;

    mod->setMotivo(ui->CmbBxmotivo->model()->data(ui->CmbBxmotivo->model()->index(ui->CmbBxmotivo->currentIndex(), 0)).toInt());
    mod->setData(ui->dateEdit->date());
    mod->setValue(ui->doubleSpinBox->value());
    mod->setTimes(ui->comboBox->currentIndex()+1);
    mod->setAll( !ui->groupBox->isChecked());
    if(ui->groupBox->isChecked())
        mod->setDweller(ui->lineEditMorador->getCurrentId());



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
       QMessageBox::warning(this, "Oops",QString("Não foi possivel salvar\n%1").arg(mod->lastError().text()));
    
}

void Editextratx::Load()
{
    if( m_mod == NULL)
      return;
    ui->CmbBxmotivo->setCurrentId(m_mod->getMotivo());

}

void Editextratx::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
extratx* Editextratx::GetSaved()
{
   return m_lastMod;

}
QList<int> Editextratx::GetSavedIds()
{
   QSqlQuery Selected;

   Selected.exec("select clientid from selectedtickets");
   Selected.first();
   for( int i = 0; i < Selected.size(); i++ )
   {
      m_selectedIds.append(Selected.record().field(0).value().toInt());
      Selected.next();
   }

   return m_selectedIds;
}


void Editextratx::RefreshListView()
{
    m_selected->setQuery(QString("select a.numero  || ' | '  || t.name  || ' | ' || d.name, d.id, d.name, a.numero, t.name, d.ramal "\
                                  "from dweller d "\
                                  "inner join selectedtickets tt on tt.clientid= d.id "\
                                  "inner join tower t on t.id= d.tower "\
                                  "inner join ap a on a.id = d.ap  where d.removed = false and d.payer = true"));

    ui->listView->setModel(m_selected);
    ui->listView->setModelColumn(0);

    ui->groupBoxSelecionados->setTitle(QString("%1 Moradores Selecionados").arg(m_selected->rowCount()));
}

void Editextratx::Add()
{
   QSqlQuery pQyeryAdd;

   pQyeryAdd.exec("CREATE TEMPORARY TABLE selectedtickets ( clientid integer );");
   pQyeryAdd.exec(QString("insert into selectedtickets(clientid) values(%1);").arg(ui->lineEditMorador->getCurrentId()));
   debug_message("Adicionado: nId= %d\n", ui->lineEditMorador->getCurrentId());
   RefreshListView();
}
void Editextratx::Remove()
{
   int nId = m_selected->index(ui->listView->currentIndex().row(),1).data().toInt();
   debug_message("Remove: nId= %d\n", nId);
   QSqlQuery pQyeryRemove;
   pQyeryRemove.exec(QString("delete from selectedtickets where clientid= %1").arg(nId));
   RefreshListView();
}

bool Editextratx::ToAll()
{
    return !ui->groupBox->isChecked();
}

void Editextratx::setVencto(QDate date)
{
    ui->dateEdit->setDate(date);
}

QDate Editextratx::getVencto()
{
    return ui->dateEdit->date();
}
void Editextratx::MovedOut()
{
    if( ui->checkBoxMovedOut->isChecked())
    {
        ui->lineEditMorador->setSelect("select a.numero  || ' | '  || t.name  || ' | ' || d.name, d.id, d.name, a.numero, t.name, d.ramal "\
                                         "from dweller d "\
                                         "inner join tower t on t.id= d.tower "\
                                         "inner join ap a on a.id = d.ap  where d.removed = false and d.payer = true");

    }
    else
    {
        ui->lineEditMorador->setSelect("select a.numero  || ' | '  || t.name  || ' | ' || d.name, d.id, d.name, a.numero, t.name, d.ramal "\
                                         "from dweller d "\
                                         "inner join tower t on t.id= d.tower "\
                                         "inner join ap a on a.id = d.ap  where d.removed = false and d.payer = true and d.movedout <> true");

    }
}
