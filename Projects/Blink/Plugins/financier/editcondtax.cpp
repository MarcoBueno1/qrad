#include "editcondtax.h"
#include "ui_Editcondtax.h"
#include <QCompleter>
#include <QMessageBox>
#include <QModelIndex>
#include <QAbstractItemModel>
#include <QVariant>
#include <QSqlField>
#include "qraddebug.h"


Editcondtx::Editcondtx(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Editcondtx)
{
    ui->setupUi(this);
    
//    m_mod = NULL;
//    m_lastMod = NULL;

    connect(ui->PshBtnSave, SIGNAL(clicked()),this,SLOT(Save()));
    connect(ui->PshBtnCancel, SIGNAL(clicked()),this,SLOT(Cancel()));
    connect(ui->pushButtonAdd, SIGNAL(clicked()),this,SLOT(Add()));
    connect(ui->pushButtonRemove, SIGNAL(clicked()),this,SLOT(Remove()));

    ui->lineEditMorador->setSelect("select a.numero  || ' | '  || t.name  || ' | ' || d.name, d.id, d.name, a.numero, t.name, d.ramal "\
                                     "from dweller d "\
                                     "inner join tower t on t.id= d.tower "\
                                     "inner join ap a on a.id = d.ap  where d.removed = false and d.payer = true");

    m_selected = new QSqlQueryModel;
}

Editcondtx::~Editcondtx()
{
    QSqlQuery removeTable;

    removeTable.exec("delete from selectedtickets");
    removeTable.exec("drop table selectedtickets");
    delete m_selected;
    delete ui;
}
void Editcondtx::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
    
    
}
void Editcondtx::keyPressEvent(QKeyEvent *e)
{
    if(e->key() != Qt::Key_Escape)
            QDialog::keyPressEvent(e);
        else {Cancel();}
}

void Editcondtx::SetModel(extratx* mod)
{
//   m_mod = mod;
//   Load();
}


void Editcondtx::Save()
{

    /*
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
       */


    accept();

}

void Editcondtx::Load()
{
//    if( m_mod == NULL)
//      return;
//    ui->CmbBxmotivo->setCurrentId(m_mod->getMotivo());

}

void Editcondtx::Cancel()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Cancelar?","Deseja cancelar esta edição?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
           reject();
}
QList<int> Editcondtx::GetSaved()
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


void Editcondtx::RefreshListView()
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

void Editcondtx::Add()
{
   QSqlQuery pQyeryAdd;

   pQyeryAdd.exec("CREATE TEMPORARY TABLE selectedtickets ( clientid integer );");
   pQyeryAdd.exec(QString("insert into selectedtickets(clientid) values(%1);").arg(ui->lineEditMorador->getCurrentId()));
   debug_message("Adicionado: nId= %d\n", ui->lineEditMorador->getCurrentId());
   RefreshListView();
}
void Editcondtx::Remove()
{
   int nId = m_selected->index(ui->listView->currentIndex().row(),1).data().toInt();
   debug_message("Remove: nId= %d\n", nId);
   QSqlQuery pQyeryRemove;
   pQyeryRemove.exec(QString("delete from selectedtickets where clientid= %1").arg(nId));
   RefreshListView();
}

bool Editcondtx::ToAll()
{
    return !ui->groupBoxOnly->isChecked();
}

void Editcondtx::setVencto(QDate date)
{
    ui->dateEditVencto->setDate(date);
}

QDate Editcondtx::getVencto()
{
    return ui->dateEditVencto->date();
}
