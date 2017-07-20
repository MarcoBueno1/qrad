#include "managerassociation.h"
#include "ui_managerassociation.h"
#include "column2delegate.h"
#include <QMessageBox>
#include <QDebug>
#include "qradcoreplugin.h"

#define PLUGIN_NAME "financier"
#define PLUGIN_NAME_ACTION_1 "NewAccountToPay"
#define PLUGIN_NAME_ACTION_2 "NewAccountToReceive"
#define PLUGIN_NAME_PARAM    "lastinsertedid"

#define BN_DEFAULT_COLUMN_SEARCH 0
//#define SQL_ITEMS "select id,id from association order by id"

Managerassociation::Managerassociation(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managerassociation)
{
    ui->setupUi(this);

    m_keyinterval = NULL;
    m_Model = new QSqlQueryModel;
    m_associatedid = 0;

//    ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->PshBtnEditar, SIGNAL(clicked()), this, SLOT(doEditar()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));

   // DoRefresh();
}

Managerassociation::~Managerassociation()
{
     if( m_keyinterval )
     {
       m_keyinterval->stop();
       delete m_keyinterval;
     }
    delete m_Model;

     delete ui;
}

void Managerassociation::setSQL(QString SQL, QRAD_ASSOCIATION_TYPE type)
{
    SQL_ITEMS = SQL;
    m_association = type;
    DoRefresh();
}

int Managerassociation::getSelectedId()
{
    return m_associatedid;
}

void Managerassociation::KeyPressTimeout()
{
//    if( ui->lineEditSearch->text().trimmed().isEmpty() )
//        ui->tableViewSearch->selectRow(0);
//    else
        ui->tableViewSearch->Search(ui->lineEditSearch->text());
}

void Managerassociation::StartTimer( QString )
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
void Managerassociation::Found(QModelIndex)
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    ShowCurrentInformations();
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    ui->tableViewSearch->SetNoEmptySearch( false);
}
void Managerassociation::notFound()
{
   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
  // ui->tableViewSearch->selectRow(0);
}
void Managerassociation::TableClicked(QModelIndex currentIndex)
{
    qDebug() << "TableClicked";
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations();
}
void Managerassociation::CurrentChanged(QModelIndex currentIndex)
{
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations( );
}

void Managerassociation::ShowCurrentInformations( void )
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
void Managerassociation::LoadTableView()
{
    QApplication::processEvents();

    m_Model->setQuery(SQL_ITEMS);

    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);

    QApplication::processEvents();
}

void Managerassociation::DoRefresh()
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

void Managerassociation::refreshTable()
{
    if(ui->lineEditSearch->text() == "")
    {
        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);
        ui->tableViewSearch->Search("");
        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
}

void Managerassociation::ConfigureTable()
{
      ui->tableViewSearch->addSearchColumnFilter(0);
    ui->tableViewSearch->addSearchColumn(0);


   // m_Model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Conuna1"));
    //m_Model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Conuna2"));
   // m_Model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Conuna3"));
   // m_Model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Conuna3"));
  //  m_Model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Conuna4"));

   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("id"));
     ui->tableViewSearch->setItemDelegateForColumn(0, new ColumnCenter);


}

void Managerassociation::keyPressEvent(QKeyEvent *event)
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

/*
void Managerassociation::MatchNewest(association *newest )
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
*/
void Managerassociation::doEditar()
{
//
//#define PLUGIN_NAME "financier"
//#define PLUGIN_NAME_ACTION_1 "NewAccountToPay"
//#define PLUGIN_NAME_ACTION_1 "NewAccountToReceive"
//#define PLUGIN_NAME_ACTION_1 "NewAccountToReceive"
//
    QRadPluginInterface *iface = QRadPluginContainer::getInstance()->plugin(PLUGIN_NAME);
    if(iface)
    {
        m_associatedid =0;
        switch(m_association)
        {
           case tpAccountToPay:
                                   iface->Process(PLUGIN_NAME_ACTION_1);
                                   m_associatedid = iface->getParam(PLUGIN_NAME_PARAM).toInt();
                                   if(m_associatedid)
                                       accept();
                                   else
                                       QMessageBox::information(this, "Oops!", "Nenhum item selecionado");
                               break;
           case tpAccountToReceive:
                               break;
        }
   }

/*
    Editassociation *edt = new Editassociation;

    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

    association *sa = association::findByPrimaryKey(nId);
    edt->SetModel(sa);
    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
*/
}

void Managerassociation::doNovo()
{
/*
    Editassociation *edt = new Editassociation;

    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
*/
}

void Managerassociation::doSair()
{
    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    m_associatedid = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

    accept();
}
