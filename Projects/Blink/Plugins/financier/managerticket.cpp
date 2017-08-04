#include "managerticket.h"
#include "ui_managerticket.h"
#include "editticket.h"
#include "column2delegate.h"
#include "financierdelegates.h"
#include <QMessageBox>
#include <QDebug>
#include <QMimeData>
#include "ticketcontroller.h"
#include "accounttoreceivehistorymodel.h"
#include "showbankreturn.h"
#include "qradplugincontainer.h"
#include <QToolBar>
#include <QMenuBar>
#include <QSqlField>
#include <QDesktopServices>
#include "pdfwrapper.h"
#include "qradprogresswindow.h"
#include "qradmoney.h"

#define BN_DEFAULT_COLUMN_SEARCH 1
#define SQL_ITEMS "select t.nossonumero as \"Nº.Banco\", t.seunumero as \"Nº.Sis\", a.numero as \"Ap\", tw.name as \"Torre\", d.name as \"Morador\", t.vencto as \"Vencto\", t.pagoem \"Pago em\", "\
                  "  t.valor as \"Valor R$\", t.valorpago as \"Pago R$\",u.name as \"Criado Por\", t.status as \"Estado\", t.sendstatus as \"e-mail\", t.type as \"Tipo\", t.id, d.id as dwellerid, t.obs, t.discount from "\
                  " ticket t inner join dweller d on d.id = t.clientid inner join vuser u on u.id = t.vuser "\
                  " inner join ap a on a.id = d.ap inner join tower tw on tw.id = d.tower and t.removed <> true"\
                  " %1 order by t.id desc; "

Managerticket::Managerticket(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Managerticket)
{
    ui->setupUi(this);

    m_keyinterval = NULL;
    m_Model = new QSqlQueryModel;

    ui->dateEditSince->setDate(QDate::currentDate());
    ui->dateEditUntil->setDate(QDate::currentDate());

//    ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 30px; min-width: 20px;};");
//    ui->tableViewSearch->setStyleSheet("QHeaderView::section {     background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1, stop:0 #3C9FE1, stop: 0.5 #308AC7, stop: 0.6 #1C79B7, stop:1 #267BB3); color: white; border: 1.1px solid #ABDEFF; min-height: 15px; min-width: 20px;}; \nselection-background-color: rgb(102, 176, 239); \nselection-color: rgb(255, 255, 255)");

    connect(ui->lineEditSearch, SIGNAL(textEdited(QString)), this, SLOT(StartTimer(QString)));
    connect(ui->tableViewSearch, SIGNAL(found(QModelIndex)), this, SLOT(Found(QModelIndex)));
    connect(ui->tableViewSearch, SIGNAL(notFound()),this,SLOT(notFound()));
    connect(ui->tableViewSearch,SIGNAL(clicked(QModelIndex)),this,SLOT(TableClicked(QModelIndex)));
    connect(ui->tableViewSearch,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(doEdit()));
    connect(ui->tableViewSearch,SIGNAL(CurrentChanged(QModelIndex)),this,SLOT(CurrentChanged(QModelIndex)));

    connect(ui->PshBtnTxExtra, SIGNAL(clicked()), this, SLOT(doTxExtra()));
    connect(ui->PshBtnTxCondominial, SIGNAL(clicked()), this, SLOT(doTxCondominial()));
    connect(ui->PshBtnSair, SIGNAL(clicked()), this, SLOT(doSair()));

    connect(ui->radioButtonAll,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonRegistered,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonRemessa,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonNotPayed,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonPayed,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->dateEditSince,SIGNAL(dateChanged(QDate)), this, SLOT(DatesChanged(QDate)));
    connect(ui->dateEditUntil,SIGNAL(dateChanged(QDate)), this, SLOT(DatesChanged(QDate)));
//    connect(ui->pushButtonReprint, SIGNAL(clicked()),this, SLOT(doReprint()));
 //   connect(ui->pushButtonEdit, SIGNAL(clicked()),this, SLOT(doEdit()));
  //  connect(ui->pushButtonRemove, SIGNAL(clicked()),this, SLOT(doRemove()));

    connect(ui->tableViewSearch,SIGNAL(OnDrop(QString)),this,SLOT(doDrop(QString)));
    connect(ui->radioButtonAllType,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonNew,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonTxCond,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->radioButtonTxExtra,SIGNAL(clicked()), this, SLOT(doRefresh()));
    connect(ui->pushButtonExportar,SIGNAL(clicked()), this, SLOT(doExport()));
    connect(ui->pushButtonImport,SIGNAL(clicked()), this, SLOT(doImport()));
 //   connect(ui->pushButtonEditDweller,SIGNAL(clicked()), this, SLOT(doEditDweller()));


    Qt::WindowFlags flags = windowFlags();
    flags |= Qt::WindowMaximizeButtonHint;
    setWindowFlags(flags);
    setWindowState(Qt::WindowMaximized);

    setAcceptDrops(true);

    createMenu();
    ui->gridLayout_4->setMenuBar(menuBar);

//    ui->tableViewSearch->setContextMenuPolicy(Qt::CustomContextMenu);
//    connect(ui->tableViewSearch, SIGNAL(customContextMenuRequested(QPoint)),
//            SLOT(customMenuRequested(QPoint)));


    DoRefresh();
    setWindowTitle("Gerenciamento de Boletos");



}

void Managerticket::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&Arquivo"), this);
    ExportAction = fileMenu->addAction(tr("Expotar (.REM/.PDF)"));
    ExportAction->setIcon(QIcon(":/png/icon_download.png"));
    ImportAction = fileMenu->addAction(tr("Importar( *.RET)"));
    ImportAction->setIcon(QIcon(":/png/icon_upload.png"));
//    ExportAction->setIcon(QIcon(":/png/edit-icon.png"));
    menuBar->addMenu(fileMenu);

    EditMenu= new QMenu(tr("&Boleto"), this);
    EditCurrent = EditMenu->addAction(tr("Editar"));
    EditCurrent->setIcon(QIcon(":/png/edit-icon.png"));
    ReporitCurrent= EditMenu->addAction(tr("Gerar PDF"));
    ReporitCurrent->setIcon(QIcon(":/png/print-icon.png"));
    EditMenu->addSeparator();
    RemoveCurrent=EditMenu->addAction(tr("Excluir"));
    RemoveCurrent->setIcon(QIcon(":/png/remove-icon.png"));
    EditMenu->addSeparator();
    EditCurrentDweller = EditMenu->addAction(tr("Editar Morador"));
    EditCurrentDweller->setIcon(QIcon(":/png/icon_id.png"));
    EditMenu->addSeparator();
    EmailCurrent = EditMenu->addAction(tr("Enviar e-mail"));
    EmailCurrent->setIcon(QIcon(":/png/icon_mail.png"));
    EditMenu->addSeparator();
    EmailToAll = EditMenu->addAction(tr("Enviar e-mail (Todos)"));
    EmailToAll->setIcon(QIcon(":/png/icon_mail.png"));
    menuBar->addMenu(EditMenu);


    PrintMenu = new QMenu(tr("&Imprimir"), this);
    PrintCurrentView = PrintMenu->addAction(tr("Listagem de tela"));
    menuBar->addMenu(PrintMenu);

    connect(ExportAction , SIGNAL(triggered()), this, SLOT(doExport()));
    connect(EditCurrentDweller , SIGNAL(triggered()), this, SLOT(doEditDweller()));
    connect(RemoveCurrent , SIGNAL(triggered()), this, SLOT(doRemove()));
    connect(ReporitCurrent , SIGNAL(triggered()), this, SLOT(doReprint()));

    connect(EditCurrent , SIGNAL(triggered()), this, SLOT(doEdit()));
    connect(EmailCurrent, SIGNAL(triggered()), this, SLOT(doEmail()));
    connect(EmailToAll, SIGNAL(triggered()), this, SLOT(doEmailToAll()));
    connect(PrintCurrentView, SIGNAL(triggered()), this, SLOT(doPrintView()));

    ui->tableViewSearch->addContextSeparator();
    ui->tableViewSearch->addContextAction(EditCurrent);
    ui->tableViewSearch->addContextAction(ReporitCurrent);
    ui->tableViewSearch->addContextSeparator();
    ui->tableViewSearch->addContextAction(RemoveCurrent);
    ui->tableViewSearch->addContextSeparator();
    ui->tableViewSearch->addContextAction(EditCurrentDweller);
    ui->tableViewSearch->addContextSeparator();
    ui->tableViewSearch->addContextAction(EmailCurrent);


    connect(ImportAction, SIGNAL(triggered()), this, SLOT(doImport()));
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

void Managerticket::TestExportPDF()
{
  refreshTable();
  ui->tableViewSearch->setCurrentIndex(m_Model->index(0,0));
  doReprint();
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
//         dwName += " - " +ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),4).data().toString();
  //       dwName.truncate(150);
  //       dwName+="...";
         EditMenu->setTitle(QString("&Edit (%1)").arg(dwName));
         //ui->groupBoxCurrent->setTitle(dwName);
    }
    else
    {
        ui->groupBoxItens->setTitle(QString(" Itens ( 0 de 0 ) "));

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
        aux = QString(" where t.status = %1 ").arg(stPaid);
        bHasWhere = true;
    }
    else if(ui->radioButtonNew->isChecked())
    {
        aux = QString(" where t.status = %1 ").arg(stCreated);
        bHasWhere = true;
    }
    else if( ui->radioButtonRemessa->isChecked())
    {
        aux = QString(" where t.status = %1 ").arg(stBuiltShipp);
        bHasWhere = true;
    }
    else if( ui->radioButtonRegistered->isChecked())
    {
        aux = QString(" where t.status = %1 ").arg(stRegistered);
        bHasWhere = true;
    }
    else
    {
        aux = QString(" where (t.status <> 3 and t.vencto < '%1') ").arg(QDate::currentDate().addDays(-3).toString(FMT_DATE_DB));
        bHasWhere = true;
//        bNeedOr = true;
    }
    QDate dtInicio;// = QDate::currentDate();
    QDate dtFim;// = QDate::currentDate();
    if(ui->groupBoxPeriod->isChecked())
    {
        dtInicio = ui->dateEditSince->date();
        dtFim    = ui->dateEditUntil->date();
        aux += QString(" %1 %2 (t.vencto between '%3' and '%4' )").arg(bHasWhere?"":" Where ").arg(!bHasWhere?"":bNeedOr?" Or ":" and ").arg(dtInicio.toString(FMT_DATE_DB)).arg(dtFim.toString(FMT_DATE_DB));
    }
    else  /// modo automatico, devem entrar atrasados e todos do dia por padrao
    {
        if(ui->radioButtonAll->isChecked())
            aux += QString(" where t.vencto = '%1' or (t.status <> 3 and t.vencto < '%1')").arg(QDate::currentDate().toString(FMT_DATE_DB));
        else
            aux += QString(" %1 %2 (t.vencto = '%3' )").arg(bHasWhere?"":" Where ").arg(!bHasWhere?"":bNeedOr?" Or ":" and ").arg(QDate::currentDate().toString(FMT_DATE_DB));

    }



    if( !ui->radioButtonAllType->isChecked())
    {
       aux += QString(" and t.type = %1 ").arg(ui->radioButtonTxCond->isChecked()?"0":"1");
    }

    strSQL = QString(SQL_ITEMS).arg(aux);

    debug_message("\nsqlquery: %s\n",strSQL.toLatin1().data());

    m_Model->setQuery(strSQL);

    QApplication::processEvents();
    ui->tableViewSearch->setModel( m_Model);
//    ui->tableViewSearch->horizontalHeader()->setStretchLastSection(true);

    QApplication::processEvents();

    double total=0;
    double totalpaid=0;
//    double totalN=0;
//    double totalpaidN=0;

    for (int index = 0; index < m_Model->rowCount(); index++)
    {
//        total     += QRadMoney::StrToInt(QRadMoney::MoneyHumanForm3(m_Model->record(index).value("Valor R$").toFloat()));
//        totalpaid += QRadMoney::StrToInt(QRadMoney::MoneyHumanForm3(m_Model->record(index).value("Pago R$").toFloat()));

        total     += m_Model->record(index).value("Valor R$").toFloat();
        totalpaid += m_Model->record(index).value("Pago R$").toFloat();
    }
//    debug_message( "Total: %02.02f  TotalPago: %02.02f\n", total,totalpaid );
//    debug_message( "TotalN: %02.02f  TotalPagoN: %02.02f\n", totalN,totalpaidN );

    ui->labelTotalEmitido->setText(QString("Total: R$ %1").arg(QRadMoney::MoneyHumanForm(total)));
    ui->labelTotalPago->setText(QString("Total Pago: R$ %1").arg(QRadMoney::MoneyHumanForm(totalpaid)));

    ui->labelSaldo->setText(QString("Saldo: R$ %1")
                            .arg(QRadMoney::MoneyHumanForm(totalpaid-total)));

    if( totalpaid>total )
        ui->labelSaldo->setStyleSheet("color: rgb(0, 122, 0);");
    else
        ui->labelSaldo->setStyleSheet("color: rgb(135, 0, 0);");
    ///color: rgb(0, 122, 0); verde ...
    ///color: rgb(135, 0, 0); vermelho
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
//        ui->tableViewSearch->sortByColumn(BN_DEFAULT_COLUMN_SEARCH, Qt::AscendingOrder);

        ui->tableViewSearch->sortByColumn(0, Qt::DescendingOrder);
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
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("dwellerid"));
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("Criado Por"));
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("obs"));
    ui->tableViewSearch->hideColumn(ui->tableViewSearch->getColumnOf("discount"));
    ui->tableViewSearch->setItemDelegateForColumn(0, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(1, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(2, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(3, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(4, new ColumnCenter);
    ui->tableViewSearch->setItemDelegateForColumn(5, new ColumnDateLate);
    ui->tableViewSearch->setItemDelegateForColumn(6, new ColumnDateTicketNull);
    ui->tableViewSearch->setItemDelegateForColumn(7, new ColumnMoney);
    ui->tableViewSearch->setItemDelegateForColumn(8, new ColumnMoneyTktPaid);
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
    this->setEnabled(false);
    TicketController *pController = new TicketController;

    if(!pController->BuildTicketExtra(0))
    {
        QMessageBox::warning( this,
                                  "Oops!",
                                  QString("Não foi possível gerar os boletos de taxa extra!!"));
    }
    else
    {
        QMessageBox::information( this,
                                  "Boletos gerdos com sucesso!",
                                  QString("Por favor pressione \Exportar\" e envie o arquivo de remessa para o banco para que os boletos tenham validade!"));

    }
    delete pController;
    doRefresh();
    this->setEnabled(true);
}

void Managerticket::doTxCondominial()
{
    this->setEnabled(false);
    TicketController *pController = new TicketController;

    if(pController->BuildTicketCond())
    {
        QMessageBox::information( this,
                                  "Boletos gerdos com sucesso!",
                                  QString("Por favor pressione \Exportar\" e envie o arquivo de remessa para o banco para que os boletos tenham validade!"));
    }

    delete pController;
    doRefresh();
    this->setEnabled(true);
}

void Managerticket::doSair()
{
    if( QMessageBox::Yes ==  QMessageBox::question(this,
                                                   "Sair?","Deseja sair desta pesquisa?",
                                                   QMessageBox::Yes | QMessageBox::No,
                                                   QMessageBox::Yes))
           reject();
}
void Managerticket::doRefresh()
{
    DoRefresh();
}

void Managerticket::doReprint()
{
    this->setEnabled(false);

    int id = ui->tableViewSearch->currentIndex().sibling( ui->tableViewSearch->currentIndex().row(),
                                                          ui->tableViewSearch->getColumnOf("id")).data().toInt();


    ticket *tkt = ticket::findByid(id,true);
    if( !tkt)
    {
        QMessageBox::warning(this, "Oops!","Selecione um boleto para poder reimprimir!");
        this->setEnabled(true);
        return;
    }
    if( tkt->getStatus() == stCreated )
    {
        QMessageBox::warning(this, "Oops!","Este boleto está em estado de \"Criado\", por favor Exporte os arquivos (.REM/.PDF)!");
        delete tkt;
        this->setEnabled(true);
        return;
    }

    debug_message("antes TicketController\n");

    TicketController *pController = new TicketController;
    if(!pController->doPrint((BBO_TYPE)tkt->getType(),(BBOL_STATUS)tkt->getStatus(), tkt))
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
    this->setEnabled(true);
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



void Managerticket::doDrop(QString path)
{
    QList<BankTicket*> list;
    ShowBankReturn *ParsePay = new ShowBankReturn ;

#ifdef _WIN32
    path = path.remove("file://");
#else
    path = "C:\\Dvl\\CN12067A.RET";
#endif

    if(ParsePay->Exec(&list, path))
    {
        /// perssit
    }

}

void Managerticket::dropEvent(QDropEvent *event)
{
    QString strPath ;

    strPath = event->mimeData()->text();


    doDrop(strPath);

}

void Managerticket::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void Managerticket::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}

void Managerticket::doExport()
{
    this->setEnabled(false);
    TicketController *pController = new TicketController;

    if(!pController->doShipp("","",tpAll))
    {
        QMessageBox::warning(this, "Oops!", "Problema ao gerar arquivo de remessa!");
        delete pController;
        this->setEnabled(true);
        return;
    }
    if( !pController->doPrint(tpAll,stCreated))
    {
        QMessageBox::warning(this, "Oops!", "Problema ao gerar arquivo de boletos!");
        delete pController;
        this->setEnabled(true);
        return;
    }
     QMessageBox::information(this, "Ok!", "Arquivos gerados com sucesso!");

     pController->OpenRemDir();
     pController->SendEmail();

     delete pController;
     this->setEnabled(true);
}

void Managerticket::doEditDweller()
{
    int id = ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         ui->tableViewSearch->getColumnOf("dwellerid")).data().toInt();

    QRadPluginContainer *pContainer = QRadPluginContainer::getInstance();

    QRadPluginInterface *iface = pContainer->plugin("visitplugin");

    if( !iface )
    {
        QMessageBox::warning(NULL,
                             QString("Oops!"),
                             QString("Não foi possível acessar o cadastro de moradores!"));
        return;
    }

    QWidget *pParent = iface->getParent();
    iface->setParent(this);

    iface->setParam("dwellerid", id);
    iface->Process("EditMorador");
    iface->setParent(pParent);

    int emailStatus = ui->tableViewSearch->currentIndex().sibling( ui->tableViewSearch->currentIndex().row(),
                                                                  ui->tableViewSearch->getColumnOf("e-mail")).data().toInt();

    if( (BBOL_SEND_STATUS)emailStatus == stDisabled )
    {
        Dweller *dw = Dweller::findByid(id);
        if(dw && dw->getNotifByEmail() && !dw->getemail().trimmed().isEmpty())
        {
            int ticketid = ui->tableViewSearch->currentIndex().sibling( ui->tableViewSearch->currentIndex().row(),
                                                                        ui->tableViewSearch->getColumnOf("t.id")).data().toInt();

            ticket *ptkt = ticket::findByid(ticketid,true);
            if(ptkt)
            {
                ptkt->updateSendStatus(stPending);
                delete ptkt;
            }

        }
    }
    doRefresh();
}

void Managerticket::doImport()
{
    this->setEnabled(false);

    QStringList paths = QRadConfig::GetAndPersistDir("RetFile", "","Selecionar Arquivo de Retorno","Arquivos Retorno (*.ret *.RET)",this);

    QList<BankTicket*> list;
    ShowBankReturn *ParsePay = new ShowBankReturn ;


    if(ParsePay->Exec(&list, paths))
    {
        TicketController::UpdateTickets(&list);
    }
    else
    {
        QMessageBox::information(NULL,"Oops!", QString("Operação cancelada!"));
    }
    delete ParsePay;
    this->setEnabled(true);

}
void Managerticket::doEmail()
{
    this->setEnabled(false);

    int id = ui->tableViewSearch->currentIndex().sibling(ui->tableViewSearch->currentIndex().row(),
                                                         ui->tableViewSearch->getColumnOf("id")).data().toInt();

    TicketController *pController = new TicketController;

    if(pController->SendEmail(id))
    {
        QMessageBox::information(NULL,"Ok!", QString("E-mail enviado com sucesso!"));
    }
    delete pController;

    this->setEnabled(true);
    doRefresh();
}

void Managerticket::doEmailToAll()
{
    this->setEnabled(false);
    TicketController *pController = new TicketController;

    pController->SendToAll();

    delete pController;
    this->setEnabled(true);
    doRefresh();
}

void Managerticket::doPrintView()
{
    QString reportTitle = ui->radioButtonPayed->isChecked()?"BOLETOS PAGOS":ui->radioButtonNotPayed->isChecked()?"INADIMPLENTES":"LISTA DE BOLETOS";

    TicketController *pController = new TicketController;

    pController->ReportExaro(m_Model,reportTitle);

    return;




    QList< FieldFormat *> headers;
    QList<QStringList *> lines;
    QStringList LeftHead;
    double dTotal = 0;

    LeftHead.append(QString("Condomínio Garden Club"));
    LeftHead.append(QString("Av Grande Otelo 270"));
    LeftHead.append(QString("Parque 10. Manaus-AM"));

    QRAD_SHOW_PRPGRESS("Inicializando Infraestrutura...");

     int i;
     QSqlQueryModel *model = m_Model;

     debug_message("antes AutoSizeColumn\n");
     AutoSizeColumn(model);

     QStringList *line;
     int nColumns = 8; //model->columnCount();
     QRAD_SHOW_PRPGRESS_STEP("Gerando cabeçalho...");

     for( int nCount=0, i = 0; i < nColumns; i++ )
     {
         debug_message("i=%d nColumns=%d\n", i, nColumns);
        if(( i==1)||( i==6))
            continue;

        QCoreApplication::processEvents();

        QString strAux = model->headerData(i, Qt::Horizontal).toString();
        if( i == 0)
            strAux="N. Banco";
        FieldFormat *f = (FieldFormat *)malloc(sizeof(FieldFormat));
        if( !f )
        {
            debug_message("não foi possivel alocar f...\n");
        }
        debug_message("strAux=%s.\n",strAux.toLatin1().data());
        strcpy(f->Name,strAux.toLatin1().data());
        debug_message("m_percents.at(i).toDouble()=%02.02f\n",m_percents.at(nCount).toDouble());
        f->Percent = m_percents.at(nCount++).toDouble();

        debug_message("antes do alignment\n");
        if( i == 4) // morador
            f->Align   = ALIGN_LEFT;
        else
            f->Align   = ALIGN_CENTER;

        debug_message("antes de adicionar\n");
        headers.append(f);
        debug_message("adicionou\n");
     }


     QRAD_SHOW_PRPGRESS_STEP("Inserindo Linhas...");
     for(  i = 0; i < model->rowCount(); i++ )
     {
       QSqlRecord rec = model->record(i);

       line = new QStringList;
       for( int j = 0; j < nColumns; j++ )
       {
        QCoreApplication::processEvents();

        if(( j==1)||( j==6))
            continue;

        if(rec.field(j).type()==QVariant::Date)
        {
             QDate field = rec.field(j).value().toDate();
             line->append(field.toString(FMT_DATE) );
        }
        else if(rec.field(j).type()==QVariant::Double)
        {
            dTotal = QRadRound::PowerRound(rec.field(j).value().toDouble()) + QRadRound::PowerRound(dTotal);
            line->append(QRadMoney::MoneyHumanForm(rec.field(j).value().toDouble() ));
        }
         else
            line->append(rec.field(j).value().toString() );
       }
       lines.append(line);

     }


     QRAD_SHOW_PRPGRESS_STEP("Construindo Arquivo...");
     debug_message("Construindo Arquivo...\n" );
     QString strAux =  QString("%1%2").arg(reportTitle).arg(".pdf");
     strAux.replace(" ", "_");
     if( 0 == pdfwrapper::Build( strAux , LeftHead, reportTitle , headers, lines, 14, QRadMoney::MoneyHumanForm(dTotal)))
     {
          QRAD_HIDE_PRPGRESS();
          debug_message("OpenURL...\n" );
          QDesktopServices::openUrl(QUrl(strAux, QUrl::TolerantMode));
     }
     QRAD_HIDE_PRPGRESS();
}


void Managerticket::AutoSizeColumn(QSqlQueryModel * model)
{
    int j;

    for( j=0; j < 8; j++)
    {
        debug_message("no loop\n");

        if(( j!=1 )&&( j!=6 ))
        {
            QString strAux = model->headerData(j, Qt::Horizontal).toString();
            double percent = (double)100/ui->tableViewSearch->width()*ui->tableViewSearch->columnWidth(j);
            debug_message("%s percent=%02.02f\n",strAux.toLatin1().data(),percent);

            m_percents.append(QString("%1").arg(percent));
        }


    }
}
void Managerticket::DatesChanged(QDate date)
{
  doRefresh();
}
