#include "managerfiles.h"
#include "ui_managerfiles.h"
#include "editfiles.h"
#include "column2delegate.h"
#include <QMessageBox>
#include <QDebug>
#include <QDesktopServices>
#include <QUrl>


#define BN_DEFAULT_COLUMN_SEARCH 2

#define SQL_ITEMS "select f.id, f.created as \"Inserido em\", f.name, f.description, f.lastaccess as \"Último Acesso\", t.description from Files f "\
            " inner join filedescription t on t.id = f.typeid where f.removed <> true and t.tp <> 2 order by id desc "


Managerfiles::Managerfiles(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managerfiles)
{
    ui->setupUi(this);

    m_keyinterval = NULL;
    m_Model = new QSqlQueryModel;

  //  ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->PshBtnEditar, SIGNAL(clicked()), this, SLOT(doEditar()));
    connect(ui->PshBtnNovo, SIGNAL(clicked()), this, SLOT(doNovo()));
    connect(ui->PshBtnRemover, SIGNAL(clicked()), this, SLOT(doRemove()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));
    connect(ui->pushButtonView, SIGNAL(clicked()), this, SLOT(doView()));
    connect(ui->tableViewSearch, SIGNAL(OnDrop(QString)), this, SLOT(doDrop(QString)));

    DoRefresh();
}

Managerfiles::~Managerfiles()
{
     if( m_keyinterval )
     {
       m_keyinterval->stop();
       delete m_keyinterval;
     }
    delete m_Model;

    delete ui;
}

void Managerfiles::KeyPressTimeout()
{
//    if( ui->lineEditSearch->text().trimmed().isEmpty() )
//        ui->tableViewSearch->selectRow(0);
//    else
        ui->tableViewSearch->Search(ui->lineEditSearch->text());
}

void Managerfiles::StartTimer( QString )
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
void Managerfiles::Found(QModelIndex)
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    ShowCurrentInformations();
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    ui->tableViewSearch->SetNoEmptySearch( false);
}
void Managerfiles::notFound()
{
   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
  // ui->tableViewSearch->selectRow(0);
}
void Managerfiles::TableClicked(QModelIndex currentIndex)
{
    qDebug() << "TableClicked";
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations();
}
void Managerfiles::CurrentChanged(QModelIndex currentIndex)
{
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations( );
}

void Managerfiles::ShowCurrentInformations( void )
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
void Managerfiles::LoadTableView()
{
    QApplication::processEvents();

    m_Model->setQuery(SQL_ITEMS);

    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);

    QApplication::processEvents();
}

void Managerfiles::DoRefresh()
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

void Managerfiles::refreshTable()
{
    if(ui->lineEditSearch->text() == "")
    {
        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);
        ui->tableViewSearch->Search("");
        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
}

void Managerfiles::ConfigureTable()
{
    ui->tableViewSearch->addSearchColumnFilter(0);
    ui->tableViewSearch->addSearchColumn(0);
    ui->tableViewSearch->addSearchColumn(1);
    ui->tableViewSearch->addSearchColumnFilter(1);

   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("id"));
    ui->tableViewSearch->setItemDelegateForColumn(1, new ColumnDate);
    ui->tableViewSearch->setItemDelegateForColumn(2, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(3, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(4, new ColumnDate);
}

void Managerfiles::keyPressEvent(QKeyEvent *event)
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
                           doSair();
                           break;
      default:
                           break;
    }


}
void Managerfiles::MatchNewest(Files *newest )
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
void Managerfiles::doEditar()
{
    Editfiles *edt = new Editfiles;

    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

    Files *sa = Files::findByPrimaryKey(nId);
    edt->SetModel(sa);
    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;

}

void Managerfiles::doNovo()
{
    Editfiles *edt = new Editfiles;

    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
}

void Managerfiles::doSair()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Sair?","Deseja sair desta pesquisa?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
           reject();
}
void Managerfiles::doRemove()
{
    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();
    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Atenção!","Tem certeza de que deseja remover este item?",QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
    {
        Files *pFile = Files::findByid(nId,true);
        if( pFile )
        {
            pFile->updateRemoved(true);
            delete pFile;
            DoRefresh();
        }
    }

}
void Managerfiles::doView()
{
    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();
    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();
    Files *pFile = Files::findByid(nId,true);
    if( pFile )
    {
        QString filename = QString("C:\\dvl\\qrad\\bin\\%1").arg(pFile->getName());
        pFile->getFile(filename,pFile->getLoId());
        QDesktopServices::openUrl(QUrl::fromLocalFile(QString("%1").arg(filename)));//, QUrl::TolerantMode);
        delete pFile;
    }

}
void ManagerfilesdoDrop(QString path)
{
    Editfiles *pNew = new Editfiles;

    pNew->setFile(path);
    pNew->exec();
    delete pNew;



}
