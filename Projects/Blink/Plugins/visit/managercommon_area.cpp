#include "managercommon_area.h"
#include "ui_managercommon_area.h"
#include "editcommon_area.h"
#include "column2delegate.h"
#include <QMessageBox>
#include <QDebug>

#define BN_DEFAULT_COLUMN_SEARCH 0
#define SQL_ITEMS "select name as \"Nome\", description as \"Descrição\",id from common_area order by id"

Managercommon_area::Managercommon_area(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managercommon_area)
{
    ui->setupUi(this);

    m_keyinterval = NULL;
    m_Model = new QSqlQueryModel;

//    ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->PshBtnEditar, SIGNAL(clicked()), this, SLOT(doEditar()));
    connect(ui->PshBtnNovo, SIGNAL(clicked()), this, SLOT(doNovo()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));

    DoRefresh();
}

Managercommon_area::~Managercommon_area()
{
     if( m_keyinterval )
     {
       m_keyinterval->stop();
       delete m_keyinterval;
     }
    delete m_Model;

    delete ui;
}

void Managercommon_area::KeyPressTimeout()
{
//    if( ui->lineEditSearch->text().trimmed().isEmpty() )
//        ui->tableViewSearch->selectRow(0);
//    else
        ui->tableViewSearch->Search(ui->lineEditSearch->text());
}

void Managercommon_area::StartTimer( QString )
{
    if( ui->lineEditSearch->text().trimmed().length() == 1 )
        ui->tableViewSearch->selectRow(0);

    if( m_keyinterval )
    {
        m_keyinterval->stop();
        delete m_keyinterval;
    }
    m_keyinterval = new QTimer;
    connect(m_keyinterval, SIGNAL(timeout()), this, SLOT(KeyPressTimeout()));
    m_keyinterval->setSingleShot(true);
    m_keyinterval->setInterval(200);
    m_keyinterval->start();
}
void Managercommon_area::Found(QModelIndex)
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    ShowCurrentInformations();
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    ui->tableViewSearch->SetNoEmptySearch( false);
}
void Managercommon_area::notFound()
{
   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
  // ui->tableViewSearch->selectRow(0);
}
void Managercommon_area::TableClicked(QModelIndex currentIndex)
{
    qDebug() << "TableClicked";
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations();
}
void Managercommon_area::CurrentChanged(QModelIndex currentIndex)
{
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations( );
}

void Managercommon_area::ShowCurrentInformations( void )
{
    QString strTemp;

    if( m_Model->rowCount() )
    {
         strTemp.sprintf(" Itens ( %d de %d ) ",
                         ui->tableViewSearch->currentIndex().row() + 1,
                         m_Model->rowCount() );
         ui->groupBoxItens->setTitle(strTemp);
    }
}
void Managercommon_area::LoadTableView()
{
    QApplication::processEvents();

    m_Model->setQuery(SQL_ITEMS);

    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);

    QApplication::processEvents();
}

void Managercommon_area::DoRefresh()
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    QApplication::processEvents();

    LoadTableView();

    ui->lineEditSearch->setFocus();
    ConfigureTable();

    QModelIndex index;
    index = ui->tableViewSearch->currentIndex();

    ui->lineEditSearch->setText(index.sibling(index.row(), BN_DEFAULT_COLUMN_SEARCH).data().toString());

    ui->tableViewSearch->SetNoEmptySearch( false );

    refreshTable();
}

void Managercommon_area::refreshTable()
{
    if(ui->lineEditSearch->text() == "")
    {
        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);
        ui->tableViewSearch->Search("");
        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
}

void Managercommon_area::ConfigureTable()
{
      ui->tableViewSearch->addSearchColumnFilter(0);
    ui->tableViewSearch->addSearchColumn(0);
    ui->tableViewSearch->addSearchColumn(1);
    ui->tableViewSearch->addSearchColumnFilter(1);


   // m_Model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Conuna1"));
    //m_Model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Conuna2"));
   // m_Model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Conuna3"));
   // m_Model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Conuna3"));
  //  m_Model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Conuna4"));

   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("id"));
     ui->tableViewSearch->setItemDelegateForColumn(0, new ColumnCenter);
     ui->tableViewSearch->setItemDelegateForColumn(1, new ColumnCenter);


}

void Managercommon_area::keyPressEvent(QKeyEvent *event)
{
    qDebug() << "KeyPressEvent";
  //  if(event->key() != Qt::Key_Escape)
    switch(event->key())
    {
      case Qt::Key_PageUp:
      case Qt::Key_Up:
      case Qt::Key_PageDown:
      case Qt::Key_Down:
      case  Qt::Key_F12:
      case Qt::Key_Delete:
                           ui->tableViewSearch->keyPressEvent(event);
                           break;
      case Qt::Key_Escape:
  //                         ui->tableViewSearch->keyPressEvent(event);
                           doSair();
                           break;
      default:
        break;
    }


}
void Managercommon_area::MatchNewest(common_area *newest )
{
    DoRefresh();
    for( int j = 0; j < newest->attributes().count(); j++ )
    {
        if( ui->tableViewSearch->model()->headerData(BN_DEFAULT_COLUMN_SEARCH,Qt::Horizontal).toString() == newest->attributes().at(j)->fieldName())
        {
            ui->lineEditSearch->setText(newest->attributes().at(j)->value().toString());
            ui->tableViewSearch->Search(newest->attributes().at(j)->value().toString());
        }
    }
}
void Managercommon_area::doEditar()
{
    Editcommon_area *edt = new Editcommon_area;

    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

    common_area *sa = common_area::findByPrimaryKey(nId);
    edt->SetModel(sa);
    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;

}

void Managercommon_area::doNovo()
{
    Editcommon_area *edt = new Editcommon_area;

    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
}

void Managercommon_area::doSair()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Sair?","Deseja sair desta pesquisa?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
           reject();
}
