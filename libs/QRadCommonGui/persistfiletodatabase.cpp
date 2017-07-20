#include "persistfiletodatabase.h"
#include "ui_persistfiletodatabase.h"
#include "column2delegate.h"
#include "files.h"


#define SQL "select f.id, f.created, f.name, f.description, f.lastaccess, t.description from files f "\
            " inner join filedescription t on t.id = f.typeid where f.removed <> true and t.tp <> 2 order by id desc "


PersistFileToDatabase::PersistFileToDatabase(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PersistFileToDatabase)
{
    ui->setupUi(this);

    m_model = new QSqlQueryModel;


    m_keyinterval = NULL;

    //ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableView, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableView, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableView,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableView,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->pushButtonAdd, SIGNAL(pressed()), this, SLOT(doAdd()));
    connect(ui->pushButtonRemove, SIGNAL(pressed()), this, SLOT(doRemove()));
    connect(ui->pushButtonView, SIGNAL(pressed()), this, SLOT(doView()));
    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(accept()));

//    m_model->setQuery(SQL);
//    ui->tableView->setModel(m_model);


    PrepareHeaders();
}

PersistFileToDatabase::~PersistFileToDatabase()
{
    delete ui;
}

void PersistFileToDatabase::PrepareHeaders()
{
    ui->tableView->addSearchColumnFilter(2);
    ui->tableView->addSearchColumnFilter(3);
    ui->tableView->addSearchColumnFilter(5);
    ui->tableView->addSearchColumn(2);
    ui->tableView->addSearchColumn(3);
    ui->tableView->addSearchColumn(5);

    ui->tableView->setItemDelegateForColumn(0, new ColumnCenter);
    ui->tableView->setItemDelegateForColumn(1, new ColumnDate);
    ui->tableView->setItemDelegateForColumn(2, new ColumnCenter);
    ui->tableView->setItemDelegateForColumn(3, new ColumnCenter);
    ui->tableView->setItemDelegateForColumn(4, new ColumnDate);
    ui->tableView->setItemDelegateForColumn(5, new ColumnCenter);
}



void PersistFileToDatabase::doAdd()
{

}
void PersistFileToDatabase::doRemove()
{
  if(QMessageBox::Yes == QMessageBox::question(this,
                                               "Atenção",
                                               "Tem certeza de que deseja remover este arquivo?",
                                               QMessageBox::Yes| QMessageBox::No, QMessageBox::No))
  {
      int id = ui->tableView->currentIndex().sibling(ui->tableView->currentIndex().row(),
                                                           ui->tableView->getColumnOf("id")).data().toInt();


      Files *f = Files::findByid(id, true);
      if(!f || f->updateRemoved(true))
      {
          QMessageBox::warning( this,
                               "Oops",
                               QString("Não foi possivel realizar esta ação: Erro %1?").arg(f->lastError().text()));
      }


  }
}

void PersistFileToDatabase::doView()
{

}
