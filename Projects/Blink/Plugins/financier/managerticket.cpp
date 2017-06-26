#include "managerticket.h"
#include "ui_managerticket.h"
#include "editticket.h"
#include "column2delegate.h"
#include "financierdelegates.h"
#include <QMessageBox>
#include <QDebug>
#include "ticketcontroller.h"
#include "editextratx.h"
#include "accounttoreceivehistorymodel.h"


#define BN_DEFAULT_COLUMN_SEARCH 1
#define SQL_ITEMS "select t.nossonumero as \"Nº.Banco\", t.seunumero as \"Nº.Sis\", a.numero as \"Ap\", tw.name as \"Torre\", d.name as \"Morador\", t.vencto as \"Vencto\", t.pagoem \"Pago em\", "\
                  " t.valor as \"Valor R$\", t.valorpago as \"Pago R$\",u.name as \"Criado Por\", t.status as \"Estado\", t.sendstatus as \"e-mail\", t.type as \"Tipo\", t.id from "\
                  " ticket t inner join dweller d on d.id = t.clientid inner join vuser u on u.id = t.vuser "\
                  " inner join ap a on a.id = d.ap inner join tower tw on tw.id = d.tower "\
                  " %1 order by id desc; "

Managerticket::Managerticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managerticket)
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

    connect(ui->PshBtnTxExtra, SIGNAL(clicked()), this, SLOT(doTxExtra()));
    connect(ui->PshBtnTxCondominial, SIGNAL(clicked()), this, SLOT(doTxCondominial()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));

    connect(ui->radioButtonAll,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonNotPayed,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonPayed,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->pushButtonReprint, SIGNAL(clicked()),this, SLOT(doReprint()));
    connect(ui->pushButtonEdit, SIGNAL(clicked()),this, SLOT(doEdit()));
    connect(ui->pushButtonRemove, SIGNAL(clicked()),this, SLOT(doRemove()));

    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
    setWindowState(Qt::WindowMaximized);
    
    ui->comboBoxMonth->setCurrentIndex(QDate::currentDate().month()-1);
    ui->comboBoxYear->setCurrentIndex(QDate::currentDate().year()-2017);

    DoRefresh();
    setWindowTitle("Gerenciamento de Boletos");
}

Managerticket::~Managerticket()
{
     if( m_keyinterval )
     {
       m_keyinterval->stop();
       delete m_keyinterval;
     }
    delete m_Model;

    delete ui;
}

void Managerticket::KeyPressTimeout()
{
//    if( ui->lineEditSearch->text().trimmed().isEmpty() )
//        ui->tableViewSearch->selectRow(0);
//    else
        ui->tableViewSearch->Search(ui->lineEditSearch->text());
}

void Managerticket::StartTimer( QString )
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
void Managerticket::Found(QModelIndex)
{
    ui->tableViewSearch->SetNoEmptySearch( true );

    ShowCurrentInformations();
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);

    ui->tableViewSearch->SetNoEmptySearch( false);
}
void Managerticket::notFound()
{
   ui->lineEditSearch->setStyleSheet(FG_COLOR_NOT_FOUND + BG_COLOR_NOT_FOUND);
  // ui->tableViewSearch->selectRow(0);
}
void Managerticket::TableClicked(QModelIndex currentIndex)
{
    qDebug() << "TableClicked";
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations();
}
void Managerticket::CurrentChanged(QModelIndex currentIndex)
{
    ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    ui->lineEditSearch->setText(currentIndex.sibling(currentIndex.row(),BN_DEFAULT_COLUMN_SEARCH).data().toString());
    ui->lineEditSearch->selectAll();

    ShowCurrentInformations( );
}

void Managerticket::ShowCurrentInformations( void )
{
    QString strTemp;

    if( m_Model->rowCount() )
    {
         strTemp.sprintf(" Itens ( %d de %d ) ",
                         ui->tableViewSearch->currentIndex().row() + 1,
                         m_Model->rowCount() );
         ui->groupBoxItens->setTitle(strTemp);


         QString dwName = ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),3).data().toString();
         dwName += " - " + ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),2).data().toString();
         dwName += " - " +ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),4).data().toString();
         dwName.truncate(150);
         dwName+="...";
         ui->groupBoxCurrent->setTitle(dwName);
    }
}
void Managerticket::LoadTableView()
{
    QApplication::processEvents();

    QString strSQL;
    QString aux ;
    bool bHasWhere = false;
    bool bNeedOr = false;

    if(ui->radioButtonAll->isChecked())
        strSQL = QString(SQL_ITEMS).arg("");
    else if( ui->radioButtonPayed->isChecked())
    {
        aux = " where t.status = 2";
        bHasWhere = true;
    }
    else
    {
        aux = QString(" where (t.status <> 2 and t.vencto < '%1') ").arg(QDate::currentDate().addDays(-3).toString(FMT_DATE_DB));
//        strSQL = QString(SQL_ITEMS).arg(aux);
        bHasWhere = true;
        bNeedOr = true;
    }
    QDate dtInicio = QDate(2017+ui->comboBoxYear->currentIndex(),  ui->comboBoxMonth->currentIndex()+1, 1);
    QDate dtFim = QDate(dtInicio.year(), dtInicio.month(),dtInicio.daysInMonth());
    aux += QString(" %1 %2 (t.vencto between '%3' and '%4' )").arg(bHasWhere?"":" Where ").arg(!bHasWhere?"":bNeedOr?" Or ":" and ").arg(dtInicio.toString(FMT_DATE_DB)).arg(dtFim.toString(FMT_DATE_DB));

    if( !ui->radioButtonAllType->isChecked())
    {
       aux += QString(" and t.type = %1 ").arg(ui->radioButtonTxCond->isChecked()?"0":"1");
    }

    strSQL = QString(SQL_ITEMS).arg(aux);



    debug_message("\nsqlquery: %s\n",strSQL.toLatin1().data());

    m_Model->setQuery(strSQL);

    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);

    QApplication::processEvents();
}

void Managerticket::DoRefresh()
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

void Managerticket::refreshTable()
{
    if(ui->lineEditSearch->text() == "")
    {
        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);
        ui->tableViewSearch->Search("");
        ui->lineEditSearch->setStyleSheet(AUTO_CONFIG_FOCUS);
    }
}

void Managerticket::ConfigureTable()
{
    ui->tableViewSearch->addSearchColumnFilter(4);
    ui->tableViewSearch->addSearchColumnFilter(3);
    ui->tableViewSearch->addSearchColumnFilter(2);
    ui->tableViewSearch->addSearchColumnFilter(1);
    ui->tableViewSearch->addSearchColumnFilter(0);
    ui->tableViewSearch->addSearchColumn(4);
    ui->tableViewSearch->addSearchColumn(3);
    ui->tableViewSearch->addSearchColumn(2);
    ui->tableViewSearch->addSearchColumn(1);
    ui->tableViewSearch->addSearchColumn(0);


   // m_Model->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Conuna1"));
    //m_Model->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Conuna2"));
   // m_Model->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Conuna3"));
   // m_Model->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Conuna3"));
  //  m_Model->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Conuna4"));

   // ui->tableViewSearch->setColumnWidth(0, 0.06 * ui->tableViewSearch->width());

    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("id"));
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("Criado Por"));
    ui->tableViewSearch->setItemDelegateForColumn(0, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(1, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(2, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(3, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(4, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(5, new ColumnDateLate);
    ui->tableViewSearch->setItemDelegateForColumn(6, new ColumnDateTicketNull);
    ui->tableViewSearch->setItemDelegateForColumn(7, new ColumnMoney);
    ui->tableViewSearch->setItemDelegateForColumn(8, new ColumnMoney);
    ui->tableViewSearch->setItemDelegateForColumn(9, new ColumnCenter);

    ui->tableViewSearch->setItemDelegateForColumn(10, new ColumnTktStatus);
    ui->tableViewSearch->setItemDelegateForColumn(11, new ColumnEmail);
    ui->tableViewSearch->setItemDelegateForColumn(12, new ColumnTktType);


}

void Managerticket::keyPressEvent(QKeyEvent *event)
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
void Managerticket::MatchNewest(ticket *newest )
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
void Managerticket::doTxExtra()
{
    /*
    Editticket *edt = new Editticket;

    QModelIndex currentIndex = ui->tableViewSearch->currentIndex();

    int nId = currentIndex.sibling(currentIndex.row(),ui->tableViewSearch->getColumnOf("id")).data().toInt();

    ticket *sa = ticket::findByPrimaryKey(nId);
    edt->SetModel(sa);
    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
   */
    Editextratx *edt = new Editextratx;
    if( QDialog::Accepted  == edt->exec())
    {
        TicketController *pt = new TicketController;
        pt->BuildTicketExtra(edt->GetSaved());

        delete pt;
        doRefresh();
    }
    delete edt;


}

void Managerticket::doTxCondominial()
{
    TicketController *pController = new TicketController;


    if(pController->BuildTicketCond())
    {
        QMessageBox::information( this,
                                  "Boletos gerdos com sucesso!",
                                  QString("Por favor envie o arquivo de remessa para o banco para que os boletos tenham validade!\nClique em \"Ok\" para abrir pasta que contem o arquivo"));

        pController->doShipp();
        pController->OpenRemDir();

        QMessageBox::information( this,
                                  "Imprimir Boletos",
                                  QString("Será aberto o arquivo de boletos. Para imprimir, por favor, verifique se a impressora está conectada e possui papel suficiente."));

        pController->doPrint(tpTxCond,stCreated);
        pController->OpenPDF();
        pController->SendEmail();
    }

    delete pController;
    doRefresh();

    /*
    Editticket *edt = new Editticket;

    if( edt->exec() == QDialog::Accepted )
    {
        MatchNewest(edt->GetSaved());
    }
    delete edt;
    */
}

void Managerticket::doSair()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this, "Sair?","Deseja sair desta pesquisa?",QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes))
           reject();
}
void Managerticket::doRefresh()
{
    DoRefresh();
}

void Managerticket::doReprint()
{

    int id = ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         ui->tableViewSearch->getColumnOf("id")).data().toInt();


    ticket *tkt = ticket::findByid(id,true);
    if( !tkt)
    {
        QMessageBox::warning(this, "Oops!","Selecione um boleto para poder reimprimir!");
        return;
    }

    TicketController *pController = new TicketController;
    if(pController->doPrint((BBO_TYPE)tkt->getType(),(BBOL_STATUS)tkt->getStatus(), tkt))
    {
        QMessageBox::warning(this, "Oops!","Problema na reimpressão do boleto!");
    }
    else
    {
        QMessageBox::information(this, "Ok!","Boleto reimpresso em PDF!\nClique em \"Ok\" para abrir a pasta que contém o arquivo.");
        pController->OpenPDF();

    }
    delete pController;
    delete tkt;
}

void Managerticket::doEdit()
{

    int id = ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         ui->tableViewSearch->getColumnOf("id")).data().toInt();

    TicketController *pController = new TicketController;

    if( pController->Edit(id) )
        doRefresh();

    delete pController;
}

void Managerticket::doRemove()
{

    int id = ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         ui->tableViewSearch->getColumnOf("id")).data().toInt();

    TicketController *pController = new TicketController;

    QString Obs = "\nAp: "+ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         2).data().toString();
    Obs += "\nTorre: "+ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         3).data().toString();
    Obs += "\nMorador: "+ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         4).data().toString();

    Obs += "\nValorR$: "+ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         7).data().toString();

    Obs += "\nVencimento: "+ QDate::fromString(ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         5).data().toString(),FMT_DATE_DB).toString(FMT_DATE);


    if(pController->Remove(id, Obs))
        doRefresh();


    delete pController;

}
