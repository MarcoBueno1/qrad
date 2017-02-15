#include "managerreason.h"
#include "ui_managerreason.h"
#include "editreason.h"
#include "column2delegate.h"
#include <QMessageBox>
#include <QDebug>

#define BN_DEFAULT_COLUMN_SEARCH 1
#define SQL_ITEMS "select id, description as \"Motivo\" from reason where description <> '' and tp = 1 and removed = false order by id"

Managerreason::Managerreason(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managerreason)
{
    ui->setupUi(this);

    m_keyinterval = NULL;
    m_Model = new QSqlQueryModel;

    ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->PshBtnEditar, SIGNAL(clicked()), this, SLOT(doEditar()));
    connect(ui->PshBtnNovo, SIGNAL(clicked()), this, SLOT(doNovo()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));

    setWindowTitle("Motivos para Visita");

    DoRefresh();
}

Managerreason::~Managerreason()
{
    if( m_keyinterval )
    {
        m_keyinterval->stop();
        delete m_keyinterval;
    }
    delete m_Model;

    delete ui;
}

void Managerreason::KeyPressTimeout()
{
//    if( ui->lineEditSearch->text().trimmed().isEmpty() )
//        ui->tableViewSearch->selectRow(0);
//    else
        ui->tableViewSearch->Search(ui->lineEditSearch->text());
}

void Managerreason::StartTimer( QString )
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
void Managerreason::Found(QModelIndex)
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    ShowCurrentInformations();
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    ui->tableViewSearch->SetNoEmptySearch( false);
}
void Managerreason::notFound()
{
   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
  // ui->tableViewSearch->selectRow(0);
}
void Managerreason::TableClicked(QModelIndex currentIndex)
{
    qDebug() << "TableClicked";
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations();
}
void Managerreason::CurrentChanged(QModelIndex currentIndex)
{
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations( );
}

void Managerreason::ShowCurrentInformations( void )
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
void Managerreason::LoadTableView()
{
    QApplication::processEvents();

    m_Model->setQuery(SQL_ITEMS);

    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);

    QApplication::processEvents();
}

void Managerreason::DoRefresh()
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

void Managerreason::refreshTable()
{
    if(ui->lineEditSearch->text() == "")
    {
        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);
        ui->tableViewSearch->Search("");
        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
}

void Managerreason::ConfigureTable()
{
      ui->tableViewSearch->addSearchColumn(1);
      ui->tableViewSearch->addSearchColumnFilter(1);


   // m_Model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Conuna1"));
    //m_Model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Conuna2"));
   // m_Model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Conuna3"));
   // m_Model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Conuna3"));
  //  m_Model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Conuna4"));

   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("id"));


}

void Managerreason::keyPressEvent(QKeyEvent *event)
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
void Managerreason::MatchNewest(reason *newest )
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
void Managerreason::doEditar()
{
    Editreason *edt = new Editreason;

    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

    reason *sa = reason::findByid(nId);
    edt->SetModel(sa);
    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;

}

void Managerreason::doNovo()
{
    Editreason *edt = new Editreason;

    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
}

void Managerreason::doSair()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Sair?","Deseja sair desta pesquisa?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
           reject();
}
