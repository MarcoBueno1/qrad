/** @file
 *
 * @page accounttoreceivereport Contas a Receber
 *
 * Informa a conta, data de lançamento, data de vencimento, data de quitação, tipo de conta, cliente, valor, valor pago e o status.
**/
#include "accounttoreceivemanager.h"
#include "ui_accounttoreceivemanager.h"
#include "qradreportmanager.h"
#include "qradconfig.h"
#include "dsmsgmessages.h"
#include "financierdelegates.h"
#include "paymentmodel.h"
#include "qraddebug.h"
#include <QCompleter>
#include "ticket.h"
#include "ticketcontroller.h"
#include "qradplugincontainer.h"

//                                    " fac.value as value, "

//// novo SQL mais inteligente
#define SQL_SELECT_ACCOUNTTORECEIVE "select fac.id, case when a.numero is null then fac.description else a.numero || ' ' || substring(t.name from 1 for 1) end as description, "\
                                    " case when c.name is NULL then 'NAO INFORMADO' else c.name end as client, "\
                                    " fac.accounttypeid, "\
                                    " fac.issuedate as issuedate,fac.vencdate as vencdate, "\
                                    " case when fac.paiddate is null then '2000-01-01' else fac.paiddate end as paiddate, "\
                                    " case when paid <> true and current_date > vencdate then "\
                                    " (fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value)+ "\
                                    " (fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value) /100* "\
                                    " (cast(replace((select juros from ticketconfig limit 1), ',','.') as double precision)/30* (current_date - vencdate)) "\
                                    " else fac.value end as value "\
                                    " ,case when fac.valuepaid is null then 0 else fac.valuepaid end as "\
                                    " valuepaid, case when fac.paid is true then 'T' else 'F' end as paid, "\
                                    " case when fac.paid = true then 'P' else case when vencdate > current_date then 'T' else "\
                                    " case when vencdate < current_date then 'V' else 'H' end end end as situation, "\
                                    " case when t.name is null then '' else t.name end as torre,"\
                                    " case when a.id is null then 0 else a.id end as apart"\
                                    " from fin_accounttoreceive fac "\
                                    " %1 join ticket tkt on tkt.accountid = fac.id %2 "\
                                    " %1 join dweller c on tkt.clientid = c.id "\
                                    " %1 join ap a on c.ap = a.id "\
                                    " %1 join tower t on c.tower = t.id %3"\
                                    " %4 where fac.removed = false %5 order by %6"
///                                        " where fac.removed = false order by t.name desc, ap.id limit 10 "


//#define SQL_SELECT_ACCOUNTTORECEIVE         "select fac.id, %1 fac.description as description,%1 case when c.name is NULL then 'NAO INFORMADO' else c.name end as cliente, %1 fac.issuedate as issuedate, %1 fac.vencdate as vencdate, %1 case when fac.paiddate is null then '2000-01-01' else fac.paiddate end as paiddate, %1 fac.value as value, %1 case when fac.valuepaid is null then 0 else fac.valuepaid end as valuepaid, %1 case when fac.paid is true then 'T' else 'F' end as paid, fac.accounttypeid, fac.clientid from fin_accounttoreceive fac %4 left join dweller c on c.id = fac.clientid where fac.removed = false %2 order by %3, fac.description"
//#define SQL_SELECT_ACCOUNTTORECEIVE_REPORT  "select fac.id, fac.description, to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate, to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate, case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate, to_char(fac.value, 'FM9G999G990D00') as value, to_char(fac.valuepaid, 'FM9G999G990D00') as valuepaid, case when fac.paid = true then 'PAGO' else 'EM ABERTO' end as status, fat.description as accounttype, case when c.name is NULL then 'NAO INFORMADO' else c.name end as client from fin_accounttoreceive fac inner join fin_accounttype fat on fat.id = fac.accounttypeid %2 left join dweller c on fac.clientid = c.id where fac.removed = false %1 order by %3"
//, fac.description"
#define SQL_DELETE_ACCOUNTTORECEIVE         "update fin_accounttoreceive set removed = true where id = %1"

#define SQL_SELECT_FORMATED                 "case when fac.paid = true then 'P' else case when vencdate > current_date then 'T' else case when vencdate < current_date then 'V' else 'H' end end end as situation"

#define CHECK_STR(str) {str += " and ";}

/**
 * Relatório de conta a receber
 *
 * @ingroup report
**/
AccountToReceiveManager::AccountToReceiveManager(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountToReceiveManager)
{
    m_ui->setupUi(this);


    m_selectAccountToReceive = new QSqlQueryModel;
    m_modelAccountType   = new QSqlQueryModel;
    m_modelClient      = new QSqlQueryModel;
    m_modelBank          = new QSqlQueryModel;

    m_orderby =-1;


    m_ui->comboBoxTypeTxExtr->setTable("reasonextratx");
    m_ui->comboBoxTypeTxExtr->setField("description");
    m_ui->comboBoxTypeTxExtr->setCanAdd(false);
    m_ui->comboBoxTypeTxExtr->setUserName("dsm");
    if( m_ui->comboBoxTypeTxExtr->completer() )
        m_ui->comboBoxTypeTxExtr->completer()->setFilterMode(Qt::MatchContains );

    m_ui->comboBoxTypeTxExtr->setVisible(false);


    m_ui->dateEditStart->setDate(QDate::currentDate().addDays(-7));
    m_ui->dateEditEnd->setDate(QDate::currentDate().addDays(7));
    m_ui->radioButtonIssueDate->setChecked(true);

    m_ui->checkBoxAccountOpen->setChecked(true);
    m_ui->checkBoxAccountPaid->setChecked(true);
    m_ui->checkBoxVencidas->setChecked(true);

    m_ui->groupBoxAccountType->setChecked(false);
    m_ui->groupBoxClient->setChecked(false);

//    InitialConfig();
    GetAccountTypeValues();
    GetClientValues();
    InitialConfig();

    m_ui->dateEditStart->setFocus();

    ViewCurrentTicket = new QAction(tr("Ver Boleto"), this);
    ViewCurrentTicket->setIcon(QIcon(":/png/edit-icon.png"));

    connect(ViewCurrentTicket , SIGNAL(triggered()), this, SLOT(doEdit()));

    EditCurrentDweller = new QAction(tr("Editar Morador"), this);
    EditCurrentDweller->setIcon(QIcon(":/png/icon_id.png"));

    connect(EditCurrentDweller , SIGNAL(triggered()), this, SLOT(doEditDweller()));



    m_ui->tableViewAccountToReceive->addContextSeparator();
    m_ui->tableViewAccountToReceive->addContextAction(ViewCurrentTicket);
    m_ui->tableViewAccountToReceive->addContextSeparator();
    m_ui->tableViewAccountToReceive->addContextAction(EditCurrentDweller);



    connect(m_ui->btnNew, SIGNAL(pressed()), this, SLOT(NewAccountToReceive()));
    connect(m_ui->btnEdit, SIGNAL(pressed()), this, SLOT(EditAccountToReceive()));
    connect(m_ui->btnPay, SIGNAL(pressed()), this, SLOT(PayAccount()));
    connect(m_ui->btnDel, SIGNAL(pressed()), this, SLOT(DeleteAccountToReceive()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(close()));
    connect(m_ui->btnReport, SIGNAL(pressed()), this, SLOT(ShowReport()));
    connect(m_ui->dateEditStart, SIGNAL(dateChanged(QDate)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->dateEditEnd, SIGNAL(dateChanged(QDate)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonIssueDate, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonVencDate, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonPaidDate, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->checkBoxAccountOpen, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->checkBoxAccountPaid, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->checkBoxVencidas, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->checkBoxPrevalecerAPTorre, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->groupBoxDate, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->groupBoxAccountType, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->groupBoxClient, SIGNAL(clicked()), this, SLOT(GetAccountToReceive()));
    connect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->comboBoxClientFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->tableViewAccountToReceive->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(fillTheFields(QModelIndex)));
    connect(m_ui->tableViewAccountToReceive, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditAccountToReceive()));
    connect(m_ui->radioButtonAllTower,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonAllType,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonGreen,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonMarine,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonOlympic,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonTxCond,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->radioButtonTxExtra,SIGNAL(clicked(bool)), this, SLOT(GetAccountToReceive()));
    connect(m_ui->comboBoxTypeTxExtr,SIGNAL(activated(int)), this, SLOT(doCmboTxExtActivacted(int)));

    m_ui->tableViewAccountToReceive->setStyleSheet("");

  //  connect(m_ui->tableViewAccountToReceive->horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sectionClicked(int)));
  //  connect(m_ui->tableViewAccountToReceive->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(sortIndicatorChanged(int,Qt::SortOrder)));

}

AccountToReceiveManager::~AccountToReceiveManager()
{
    delete m_selectAccountToReceive;
    delete m_ui;
    delete m_modelAccountType;
    delete m_modelClient;
    delete m_modelBank;
    delete ViewCurrentTicket;
    delete EditCurrentDweller;
}

void AccountToReceiveManager::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void AccountToReceiveManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);
/*
    GetAccountTypeValues();
    GetClientValues();
    InitialConfig();

    m_ui->dateEditStart->setFocus();
    */
}

void AccountToReceiveManager::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ConfigHeaderTable();
}

void AccountToReceiveManager::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, MSG_INFO_TITLE, MSG_QUESTION_CLOSE, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    {
        event->ignore();
    }
}

void AccountToReceiveManager::GetAccountToReceive(void)
{
   QString JoinType = "left";
   QString TicketType;
   QString Tower;
   QString ExtraTaxJoin;
   QString Where;
   QString OrderBy;
   QString PrevalecOrderBy;
   QString WhereVencidas;

   if( m_ui->checkBoxPrevalecerAPTorre->isChecked())
   {
        PrevalecOrderBy = " t.name desc, a.id, ";
   }




//    " %1 join ticket tkt on tkt.accountid = fac.id %2 "
//    " %1 join dweller c on tkt.clientid = c.id "
//    " %1 join ap a on c.ap = a.id "
//    " %1 join tower t on c.tower = t.id %3"
//    " %4 where fac.removed = false %5 %6"

//    m_InnerJoinTicket ="";
    m_strAux = "";
    if (m_ui->radioButtonIssueDate->isChecked())
    {
        OrderBy += "fac.issuedate";
        PrevalecOrderBy += "fac.issuedate";
    }
    else if (m_ui->radioButtonVencDate->isChecked())
    {
        OrderBy += "fac.vencdate";
        PrevalecOrderBy +="fac.vencdate";
    }
    else
    {
        OrderBy += "fac.paiddate";
        PrevalecOrderBy += "fac.paiddate";
    }

    if (m_ui->groupBoxDate->isChecked())
    {
        CHECK_STR(Where);
        Where += QString(" %1 between '%2' and '%3' ")
                                .arg(OrderBy)
                                .arg(m_ui->dateEditStart->date().toString(FMT_DATE_DB))
                                .arg(m_ui->dateEditEnd->date().toString(FMT_DATE_DB));
    }

    if ( (m_ui->checkBoxAccountOpen->isChecked()) && (m_ui->checkBoxAccountPaid->isChecked()) )
    {
        /** nao adiciona nada */
    }
    else if (m_ui->checkBoxAccountOpen->isChecked())
    {
        CHECK_STR(Where);
        Where += " fac.paid = false ";
    }
    else if (m_ui->checkBoxAccountPaid->isChecked())
    {
        CHECK_STR(Where);
        Where += " fac.paid = true ";
    }
    else
    {
        CHECK_STR(Where);
        Where += " fac.paid = true and fac.paid = false ";
    }

    if (m_ui->groupBoxAccountType->isChecked())
    {
        int accountTypeId;

        CHECK_STR(Where);

        disconnect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));

        GET_COMBOBOX_ID(accountTypeId, m_ui->comboBoxAccountTypeFilter);
        Where += QString(" fac.accounttypeid = %1 ").arg(accountTypeId);
        WhereVencidas += QString(" and fac.accounttypeid = %1 ").arg(accountTypeId);

        connect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));
    }

    if (m_ui->groupBoxClient->isChecked())
    {
        int clientId;

        CHECK_STR(Where);

        disconnect(m_ui->comboBoxClientFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));

        GET_COMBOBOX_ID(clientId, m_ui->comboBoxClientFilter);
        Where += QString(" fac.clientid = %1 ").arg(clientId);
        WhereVencidas += QString(" and fac.clientid = %1 ").arg(clientId);

        connect(m_ui->comboBoxClientFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));
    }

    //// inner join ticket ....
    if( m_ui->radioButtonOlympic->isChecked())
    {
//       m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id  ";
//       m_InnerJoinTicket +=  " inner join dweller dw on dw.id = tkt.clientid and dw.tower = 1 ";
         Tower = " and t.id = 1";
         JoinType = " inner ";
    }
    else if( m_ui->radioButtonMarine->isChecked())
    {
//       m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id  ";
//       m_InnerJoinTicket +=  " inner join dweller dw on dw.id = tkt.clientid and dw.tower = 2 ";
        Tower = " and t.id = 2";
        JoinType = " inner ";
    }
    else if( m_ui->radioButtonGreen->isChecked())
    {
//       m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id ";
//       m_InnerJoinTicket +=  " inner join dweller dw on dw.id = tkt.clientid and dw.tower = 3 ";
        Tower = " and t.id = 3";
        JoinType = " inner ";
    }

    if( m_ui->radioButtonAllType->isChecked())
        m_ui->comboBoxTypeTxExtr->setVisible(false);
    if( m_ui->radioButtonTxCond->isChecked())
    {
        m_ui->comboBoxTypeTxExtr->setVisible(false);
//        if(m_InnerJoinTicket.isEmpty())
//            m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id and tkt.type = 0 ";
//        else
//            m_InnerJoinTicket.insert(strlen(" inner join ticket tkt on tkt.accountid = fac.id ")," and tkt.type = 0 ");
        TicketType = " and tkt.type = 0 ";
        JoinType = " inner ";

    }
    else if( m_ui->radioButtonTxExtra->isChecked())
    {
//        QString aux2;
        if(!m_ui->comboBoxTypeTxExtr->isVisible())
        {
            m_ui->comboBoxTypeTxExtr->setVisible(true);
        }
        int currentid = m_ui->comboBoxTypeTxExtr->model()->data(m_ui->comboBoxTypeTxExtr->model()->index(m_ui->comboBoxTypeTxExtr->currentIndex(), 0)).toInt();
        debug_message("Current index of type of extratax: %d , currentindex=%d........\n", currentid, m_ui->comboBoxTypeTxExtr->currentIndex());
        if( currentid > 0 )
        {
            ExtraTaxJoin = QString(" inner join extratx ext on ext.id = tkt.extratxid and ext.motivo = %1 ").arg(currentid);
        }
        JoinType = " inner ";


//        if(m_InnerJoinTicket.isEmpty())
//            m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id and tkt.type = 1 " + aux2;
//        else
//            m_InnerJoinTicket.insert(strlen(" inner join ticket tkt on tkt.accountid = fac.id ")," and tkt.type = 1 " + aux2);
    }

    QString TableViewQuery;

    if( m_ui->checkBoxVencidas->isChecked() )
    {
        QString WhereUnion = QString(" and ( fac.paid = false and fac.vencdate < '%1') %2 ")
                .arg(QDate::currentDate().toString(FMT_DATE_DB))
                .arg(WhereVencidas);
        QString Union = "( " +  QString(SQL_SELECT_ACCOUNTTORECEIVE)
                .arg(JoinType)
                .arg(TicketType)
                .arg(Tower)
                .arg(ExtraTaxJoin)
                .arg(WhereUnion)
                .arg(PrevalecOrderBy);

        TableViewQuery  =  QString(" %1) union ( ")
                .arg(Union);

        TableViewQuery += QString(SQL_SELECT_ACCOUNTTORECEIVE)
                .arg(JoinType)
                .arg(TicketType)
                .arg(Tower)
                .arg(ExtraTaxJoin)
                .arg(Where)
                .arg(PrevalecOrderBy);
        TableViewQuery += QString(" ) ") + QString("order by ") + PrevalecOrderBy.replace("t.name", "torre").replace("c.name", "client").replace("fac.paid", "status").replace("a.id", "apart").remove("fac.");
    }
    else
    {

        TableViewQuery = QString(SQL_SELECT_ACCOUNTTORECEIVE)
                .arg(JoinType)
                .arg(TicketType)
                .arg(Tower)
                .arg(ExtraTaxJoin)
                .arg(Where)
                .arg(PrevalecOrderBy);
    }




    TableViewQuery = TableViewQuery.replace("fac.accounttypeid,",
                                            "case when tkt.type is null then fat.description else case when tkt.type = 0 then 'CONDOMÍNIO' else 'TX EXTRA' end end as accounttype,");
    TableViewQuery = TableViewQuery.replace( "from fin_accounttoreceive fac",
                                             "from fin_accounttoreceive fac inner join fin_accounttype fat on fat.id = fac.accounttypeid ");


    m_selectAccountToReceive->setQuery(TableViewQuery);

    QString strDebug = QString(TableViewQuery);

    debug_message("\nSQL_SELECT_ACCOUNTTORECEIVE=%s\n", strDebug.toLatin1().data());
    m_ui->tableViewAccountToReceive->setModel(m_selectAccountToReceive);
 //   m_ui->tableViewAccountToReceive->show();
    m_ui->tableViewAccountToReceive->selectRow(0);


    m_ui->groupBoxAccounts->setTitle(QString("Contas a Receber(%1)").arg(m_selectAccountToReceive->rowCount()));

    double total = 0;
    double totalpaid = 0;
    for (int index = 0; index < m_selectAccountToReceive->rowCount(); index++)
    {
        QString strAux = m_selectAccountToReceive->record(index).value("value").toString();
        total     = QRadRound::PowerRound(total) + QRadRound::PowerRound(strAux.remove("H").remove("P").remove("V").remove("T").toFloat());
        strAux = m_selectAccountToReceive->record(index).value("valuepaid").toString();
        totalpaid = QRadRound::PowerRound(totalpaid) + QRadRound::PowerRound(strAux.remove("H").remove("P").remove("V").remove("T").toFloat());
    }
    debug_message( "Total: %02.02f  TotalPago: %02.02f\n", total,totalpaid );

    m_ui->labelPagar->setText(QString("Total: %1").arg(QRadMoney::MoneyHumanForm(total)));
    m_ui->labelPago->setText(QString("Total Pago: %1").arg(QRadMoney::MoneyHumanForm(totalpaid)));

    m_ui->labelSaldo->setText(QString("Saldo: %1")
                            .arg(QRadMoney::MoneyHumanForm(QRadRound::PowerRound(totalpaid) - QRadRound::PowerRound(total))));

    if( totalpaid>=total )
        m_ui->labelSaldo->setStyleSheet("color: rgb(0, 122, 0);");
    else
        m_ui->labelSaldo->setStyleSheet("color: rgb(135, 0, 0);");

}

void AccountToReceiveManager::GetAccountTypeValues(void)
{
    m_modelAccountType->setQuery(QString(SQL_SELECT_ACCOUNTTYPE_COMBO).arg(AccountTypeToReceive).arg(AccountTypeBoth));
    m_ui->comboBoxAccountType->setModel(m_modelAccountType);
    m_ui->comboBoxAccountType->setModelColumn(1);

    m_ui->comboBoxAccountTypeFilter->setModel(m_modelAccountType);
    m_ui->comboBoxAccountTypeFilter->setModelColumn(1);
}

void AccountToReceiveManager::GetClientValues(void)
{
    m_modelClient->setQuery(SQL_SELECT_CLIENT_COMBO_PAYER);
//    m_ui->comboBoxClient->setModel(m_modelClient);
//    m_ui->comboBoxClient->setModelColumn(1);

    m_ui->comboBoxClientFilter->setModel(m_modelClient);
    m_ui->comboBoxClientFilter->setModelColumn(1);
}

void AccountToReceiveManager::InitialConfig(int row)
{
    GetAccountToReceive();
    QModelIndex currentIndex = m_ui->tableViewAccountToReceive->currentIndex();

    ConfigHeaderTable();
    m_ui->tableViewAccountToReceive->setCurrentIndex(m_selectAccountToReceive->index(0,1));

    if (row != 0)
    {
        m_ui->tableViewAccountToReceive->selectRow(row);
    }
    else
    {
        m_ui->tableViewAccountToReceive->setCurrentIndex(m_selectAccountToReceive->index(currentIndex.row(),1));
    }
}

void AccountToReceiveManager::ConfigHeaderTable(void)
{
    m_ui->tableViewAccountToReceive->hideColumn(0);
//    m_ui->tableViewAccountToReceive->hideColumn(9);
    m_ui->tableViewAccountToReceive->hideColumn(10);
    m_ui->tableViewAccountToReceive->hideColumn(11);
    m_ui->tableViewAccountToReceive->hideColumn(12);
    m_ui->tableViewAccountToReceive->hideColumn(13);

    m_selectAccountToReceive->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Descrição/AP Torre"));
    m_selectAccountToReceive->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Devedor / Condômino"));
    m_selectAccountToReceive->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Tipo"));
    m_selectAccountToReceive->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Data de Lançamento"));
    m_selectAccountToReceive->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Data de Vencimento"));
    m_selectAccountToReceive->setHeaderData(6, Qt::Horizontal, QString::fromUtf8("Data de Quitação"));
    m_selectAccountToReceive->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Valor"));
    m_selectAccountToReceive->setHeaderData(8, Qt::Horizontal, QString::fromUtf8("Valor Pago"));
    m_selectAccountToReceive->setHeaderData(9, Qt::Horizontal, QString::fromUtf8("Status"));

    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(1, new ColumnFinancierDescription);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(2, new ColumnFinancierDescription);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(3, new ColumnFinancierDescription);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(4, new ColumnFinancierDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(5, new ColumnFinancierDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(6, new ColumnFinancierDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(7, new ColumnFinancierMoney);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(8, new ColumnFinancierMoney);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(9, new ColumnFinancierAccountPaid);

//    m_ui->tableViewAccountToReceive->setColumnWidth(1, 0.25 * m_ui->tableViewAccountToReceive->width());
//    m_ui->tableViewAccountToReceive->setColumnWidth(2, 0.15 * m_ui->tableViewAccountToReceive->width());
//    m_ui->tableViewAccountToReceive->setColumnWidth(3, 0.11 * m_ui->tableViewAccountToReceive->width());
//    m_ui->tableViewAccountToReceive->setColumnWidth(4, 0.11 * m_ui->tableViewAccountToReceive->width());
//   m_ui->tableViewAccountToReceive->setColumnWidth(5, 0.10 * m_ui->tableViewAccountToReceive->width());
//    m_ui->tableViewAccountToReceive->setColumnWidth(6, 0.10 * m_ui->tableViewAccountToReceive->width());
//    m_ui->tableViewAccountToReceive->setColumnWidth(7, 0.10 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->horizontalHeader()->setStretchLastSection(true);
}

void AccountToReceiveManager::fillTheFields(QModelIndex currentIndex)
{
    m_accountToReceiveId = currentIndex.sibling(currentIndex.row(),0).data().toInt();
    m_accountToReceiveRow = m_ui->tableViewAccountToReceive->currentIndex().row();
    m_accountToReceivePaid = (currentIndex.sibling(currentIndex.row(),9).data().toString() == "T")?true:false;

    if (m_accountToReceivePaid)
    {
        m_ui->btnPay->setText(QString::fromUtf8("Es&tornar"));
    }
    else
    {
        m_ui->btnPay->setText(QString::fromUtf8("&Quitar"));
    }


    m_ui->comboBoxAccountType->setModelColumn(0);
//    m_ui->comboBoxClient->setModelColumn(0);

    m_ui->comboBoxAccountType->setCurrentIndex(m_ui->comboBoxAccountType->findText(QString("%1").arg(currentIndex.sibling(currentIndex.row(),9).data().toInt())));
//    m_ui->comboBoxClient->setCurrentIndex(m_ui->comboBoxClient->findText(QString("%1").arg(currentIndex.sibling(currentIndex.row(),10).data().toInt())));

    m_ui->comboBoxAccountType->setModelColumn(1);
//    m_ui->comboBoxClient->setModelColumn(1);
}

void AccountToReceiveManager::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
        {
            this->close();
            break;
        }

        default:
            QWidget::keyPressEvent( event );
            break;
    }
}

void AccountToReceiveManager::NewAccountToReceive(void)
{
    AccountToReceiveRegister *accountToReceiveRegister = new AccountToReceiveRegister(this);
    QRadConfig::centralizarWidget(accountToReceiveRegister);

    if (accountToReceiveRegister->exec() == QDialog::Accepted)
    {
        AccountToReceiveModel *accountToReceiveModel = new AccountToReceiveModel;
        AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

        accountToReceiveHistoryModel->setAccountToReceiveId(accountToReceiveModel->lastInsertId());
        accountToReceiveHistoryModel->setTypeOperation(AccountOpCreate);
        accountToReceiveHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
        accountToReceiveHistoryModel->setDate(QDate::currentDate());
        accountToReceiveHistoryModel->setTime(QTime::currentTime());

        accountToReceiveHistoryModel->Create();

        delete accountToReceiveModel;
        delete accountToReceiveHistoryModel;
    }
    InitialConfig(m_accountToReceiveRow);

    delete accountToReceiveRegister;
}

void AccountToReceiveManager::EditAccountToReceive(void)
{
    if (m_selectAccountToReceive->rowCount() > 0)
    {
        AccountToReceiveRegister *accountToReceiveRegister = new AccountToReceiveRegister(this);
        QRadConfig::centralizarWidget(accountToReceiveRegister);

        accountToReceiveRegister->SendAccountToReceiveId(m_accountToReceiveId);

        if (accountToReceiveRegister->exec() == QDialog::Accepted)
        {
            AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

            accountToReceiveHistoryModel->setAccountToReceiveId(m_accountToReceiveId);
            accountToReceiveHistoryModel->setTypeOperation(AccountOpEdit);
            accountToReceiveHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
            accountToReceiveHistoryModel->setDate(QDate::currentDate());
            accountToReceiveHistoryModel->setTime(QTime::currentTime());

            accountToReceiveHistoryModel->Create();

            delete accountToReceiveHistoryModel;
        }
        InitialConfig(m_accountToReceiveRow);

        delete accountToReceiveRegister;
    }
}

void AccountToReceiveManager::PayAccount(void)
{
    debug_message("-->\n");
    if (m_selectAccountToReceive->rowCount() > 0)
    {
        if (m_accountToReceivePaid)
        {
            if (QMessageBox::question(this, MSG_QUESTION_TITLE, MSG_QUESTION_RESTORE_ACCOUNT, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {

                ticket *tkt = ticket::findByAccountId(m_accountToReceiveId,true);
                if(tkt)
                {
                    tkt->setPagoEm(QDate(2000,1,1));
                    tkt->setValorPago(0);
                    tkt->setStatus(stRegistered); /// verificar necessidade de saber exatamente qual o estado anterior.
                    tkt->Save();
                }

                AccountToReceiveModel *accountToReceiveModel = new AccountToReceiveModel;

                accountToReceiveModel->setId(m_accountToReceiveId);

                accountToReceiveModel->updatePaid(false);
                accountToReceiveModel->updateValuePaid(0);
                accountToReceiveModel->updatePaidDate(QDate(2000,1,1));

                delete accountToReceiveModel;

                AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

                accountToReceiveHistoryModel->setAccountToReceiveId(m_accountToReceiveId);
                accountToReceiveHistoryModel->setTypeOperation(AccountOpRestore);
                accountToReceiveHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
                accountToReceiveHistoryModel->setDate(QDate::currentDate());
                accountToReceiveHistoryModel->setTime(QTime::currentTime());

                accountToReceiveHistoryModel->Create();

                delete accountToReceiveHistoryModel;


                QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
                InitialConfig(m_accountToReceiveRow);
            }
        }
        else
        {
            PaidAccount *paidAccount = new PaidAccount(this);
            QRadConfig::centralizarWidget(paidAccount);

//            m_selectAccountToReceive->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Valor"));
            QString strValueToPay = m_selectAccountToReceive->index(m_ui->tableViewAccountToReceive->currentIndex().row(),7).data().toString();

            if( strValueToPay.contains("."))
                strValueToPay = strValueToPay.remove(".");
            strValueToPay.replace(",", ".");

            paidAccount->SendPaidAccountId(m_accountToReceiveId, AccountTypeToReceive, strValueToPay.toDouble());

            debug_message("-->\n");


            if (paidAccount->exec() == QDialog::Accepted)
            {
                ////
                /// dar baixa no ticket tambem
                ///

                ticket *tkt = ticket::findByAccountId(m_accountToReceiveId,true);
                if(tkt)
                {
                    tkt->setPagoEm(QDate::currentDate());
                    tkt->setValorPago(paidAccount->getValuePaid());
                    tkt->setStatus(stPaid);
                    tkt->Save();
                }
                debug_message("-->\n");

                AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

                accountToReceiveHistoryModel->setAccountToReceiveId(m_accountToReceiveId);
                accountToReceiveHistoryModel->setTypeOperation(AccountOpPaid);
                accountToReceiveHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
                accountToReceiveHistoryModel->setDate(QDate::currentDate());
                accountToReceiveHistoryModel->setTime(QTime::currentTime());

                debug_message("-->\n");

                accountToReceiveHistoryModel->Create();
                debug_message("-->\n");

/*
                AccountToReceiveModel *account = AccountToReceiveModel::findByPrimaryKey(m_accountToReceiveId);
                Debt *debt = account->getDebt();
                QSqlQueryModel paymentId;
                paymentId.setQuery(QString("select id from payment where debtid = %1 order by date desc").arg(account->getDebtId()));

                if(!debt->getPaid())
                {
                    Payment *payment = new Payment;
                    payment->setDate(QDate::currentDate());
                    payment->setTime(QTime::currentTime());
                    payment->setDebtId(account->getDebtId());
                    payment->setValue(account->getValuePaid());
                    payment->setUserId(QRadConfig::GetCurrentUserId());
                    payment->Create();
                    debt->setPaid(true);//checkPaid();
                    delete payment;
                }
*/

                delete accountToReceiveHistoryModel;
                debug_message("-->\n");


            }
            InitialConfig(m_accountToReceiveRow);
            debug_message("-->\n");


            delete paidAccount;

            debug_message("-->\n");

        }
    }
}

void AccountToReceiveManager::DeleteAccountToReceive(void)
{
    if (m_selectAccountToReceive->rowCount() > 0)
    {
//        QModelIndex index = m_ui->tableViewAccountToReceive->currentIndex();

        if (QMessageBox::question(this, MSG_QUESTION_TITLE, MSG_QUESTION_DELETE_ACCOUNTTOPAY, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
        {
            AccountToReceiveModel *accountToReceiveModel = new AccountToReceiveModel;

            accountToReceiveModel->setId(m_accountToReceiveId);
            accountToReceiveModel->updateRemoved(true);

            delete accountToReceiveModel;

            AccountToReceiveHistoryModel *accountToReceiveHistoryModel = new AccountToReceiveHistoryModel;

            accountToReceiveHistoryModel->setAccountToReceiveId(m_accountToReceiveId);
            accountToReceiveHistoryModel->setTypeOperation(AccountOpRemove);
            accountToReceiveHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
            accountToReceiveHistoryModel->setDate(QDate::currentDate());
            accountToReceiveHistoryModel->setTime(QTime::currentTime());

            accountToReceiveHistoryModel->Create();

            delete accountToReceiveHistoryModel;

            QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_DELETE_OK);
            InitialConfig(m_accountToReceiveRow);
        }
    }
}

void AccountToReceiveManager::ShowReport(void)
{
    this->setEnabled(false);
    enum enOrderBy
    {
        ObDescription=1,
        ObCliente,
        ObIssuedate,
        ObVencdate,
        ObPaiddate,
        ObValue,
        ObValuePaid,
        ObPaid
    };


    if (m_selectAccountToReceive->rowCount() > 0)
    {
        QSqlQueryModel *select = new QSqlQueryModel;
        QRadReportManager   *report = new QRadReportManager();
        int total = 0, totalPaid = 0;

        if ( !report->load( "accounttoreceive" ) )
        {
           QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo modelo." );
           delete report;
           this->setEnabled(true);
           return;
        }

        QString OrderBy;
        switch(m_orderby)
        {
          case ObDescription:
            OrderBy = "fac.description ";
            if(m_NeedDesc)
                OrderBy += " desc ";
            break;
          case ObCliente:
            OrderBy = "client ";
            if(m_NeedDesc)
                OrderBy += " desc ";
            break;
          case ObIssuedate:
            OrderBy = "issuedate ";
            if(m_NeedDesc)
                OrderBy += " desc ";
            break;
          case ObVencdate:
            OrderBy = "vencdate ";
            if(m_NeedDesc)
                OrderBy += " desc ";
            break;
          case ObPaiddate:
            OrderBy = "paiddate ";
            if(m_NeedDesc)
                OrderBy += " desc ";
            break;
          case ObValue:
            OrderBy = "value ";
            if(m_NeedDesc)
                OrderBy += " desc ";
            break;
          case ObValuePaid:
            OrderBy = "valuepaid ";
            break;
          case ObPaid:
            OrderBy = "status ";
            if(!m_NeedDesc)
                OrderBy += " desc ";
            debug_message("OrderBy = %s\n",OrderBy.toLatin1().data());
            break;
          default:
            OrderBy = m_dateStr;
            OrderBy +=", fac.description ";
            debug_message("OrderBy = %s\n",OrderBy.toLatin1().data());
            break;
        }


        ///////
        /// \brief nova logica para aproveitar SQL principal
        ///
        QString TableViewQuery = m_selectAccountToReceive->query().lastQuery();


        TableViewQuery = TableViewQuery.replace("fac.issuedate as issuedate,","to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate,");
        TableViewQuery = TableViewQuery.replace("fac.vencdate as vencdate,","to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate,");
        TableViewQuery = TableViewQuery.replace("case when fac.paiddate is null then '2000-01-01' else fac.paiddate end as paiddate,","case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate,");
//        TableViewQuery = TableViewQuery.replace("fac.value as value,","to_char(fac.value, 'FM9G999G990D00') as value,");
        TableViewQuery = TableViewQuery.replace(" case when paid <> true and current_date > vencdate then "\
        " (fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value)+ "\
        " (fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value) /100* "\
        " (cast(replace((select juros from ticketconfig limit 1), ',','.') as double precision)/30* (current_date - vencdate)) "\
        " else fac.value end as value ",

        " case when paid <> true and current_date > vencdate then "\
        " to_char((fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value)+ "\
        " (fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value) /100* "\
        " (cast(replace((select juros from ticketconfig limit 1), ',','.') as double precision)/30* (current_date - vencdate)), 'FM9G999G990D00')"\
        " else to_char(fac.value, 'FM9G999G990D00') end as value ");





        TableViewQuery = TableViewQuery.replace("case when fac.paid is true then 'T' else 'F' end as paid,",
                                                "case when fac.paid = true then 'PAGO' when current_date > fac.vencdate then 'VENCIDA' else 'A VENCER' end as status,");
        TableViewQuery = TableViewQuery.replace("case when fac.valuepaid is null then 0 else fac.valuepaid end as  valuepaid,","to_char(fac.valuepaid, 'FM9G999G990D00') as valuepaid,");
        //TableViewQuery = TableViewQuery.replace("fac.accounttypeid,","case when tkt.type is null then fat.description else case when tkt.type = 0 then 'CONDOMÍNIO' else 'TX EXTRA' end end as accounttype,");
        //TableViewQuery = TableViewQuery.replace( "from fin_accounttoreceive fac",  "from fin_accounttoreceive fac inner join fin_accounttype fat on fat.id = fac.accounttypeid ");


        //debug_message("OrderBy = %s\n",OrderBy.toLatin1().data());

        select->setQuery(TableViewQuery);

//        debug_message("SQL: %s\n", QString(SQL_SELECT_ACCOUNTTORECEIVE_REPORT)
//                      .arg(m_strAux)
//                     // .arg(m_dateStr)
//                      .arg(m_InnerJoinTicket)
//                      .arg(OrderBy).toLatin1().data());

        debug_message("SQL Report:%s\n",TableViewQuery.toLatin1().data());

        for (int index = 0; index < select->rowCount(); index++)
        {
            total       += QRadMoney::StrToInt(select->record(index).value("value").toString());
            totalPaid   += QRadMoney::StrToInt(select->record(index).value("valuepaid").toString());
        }

//        report->setQuery("account", QString(SQL_SELECT_ACCOUNTTORECEIVE_REPORT)
  //                                              .arg(m_strAux)
    //                   //                         .arg(m_dateStr)
      //                                          .arg(m_InnerJoinTicket)
        //                                        .arg(OrderBy));

          report->setQuery("account", TableViewQuery);

        report->setAttributeMoneyValue("TOTAL", total);
        report->setAttributeMoneyValue("TOTAL_PAID", totalPaid);

        if ( !report->show() )
        {
            QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
        }

        delete report;
        delete select;
    }
    this->setEnabled(true);

}
void AccountToReceiveManager::doCmboTxExtActivacted(int item)
{
  Q_UNUSED(item);

  GetAccountToReceive();
}

void AccountToReceiveManager::doEdit()
{

    int accountid = m_ui->tableViewAccountToReceive->currentIndex().sibling(m_ui->tableViewAccountToReceive->currentIndex().row(),
                                                         m_ui->tableViewAccountToReceive->getColumnOf("id")).data().toInt();

    ticket *tkt = ticket::findByAccountId(accountid,true);
    if( !tkt )
    {
        QMessageBox::warning(this, "Oops!", "Não existe boleto associado a esta conta!");
        return;
    }

    TicketController *pController = new TicketController;

    pController->Edit(tkt->getid(), true);

    delete pController;
    delete tkt;
}
void AccountToReceiveManager::doEditDweller()
{
    int id = m_ui->tableViewAccountToReceive->currentIndex().sibling(m_ui->tableViewAccountToReceive->currentIndex().row(),
                                                         m_ui->tableViewAccountToReceive->getColumnOf("id")).data().toInt();

    AccountToReceiveModel *ac = AccountToReceiveModel::findById(id,true);
    if(!ac)
    {
        QMessageBox::warning(this, "Oops!", "Não foi possível encontrar a conta atual!, por favor selecione uma conta!");
        return;
    }
    int dwellerid = ac->getClientId();
    if( !dwellerid )
    {
        ticket *tkt = ticket::findByAccountId(id,true);
        if( !tkt )
        {
            QMessageBox::warning(this, "Oops!", "Não foi possível encontrar o Morador!");
            delete ac;
            return;
        }
        dwellerid = tkt->getclientid();
        delete tkt;
    }


    delete ac;

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

    GetAccountToReceive();
}

void AccountToReceiveManager::Test()
{
 // set
    m_ui->radioButtonTxExtra->setChecked(true);
//2017-06-25
    m_ui->dateEditStart->setDate(QDate(2017,6,25));

    m_ui->comboBoxTypeTxExtr->setCurrentIndex(1);

    GetAccountToReceive();

    ShowReport();
}

void AccountToReceiveManager::sectionClicked(int orderby)
{

}

void AccountToReceiveManager::sortIndicatorChanged(int orderby,Qt::SortOrder sortOrder)
{
    debug_message("OrderBy=%d sortOrder=%d\n", orderby, sortOrder);
    m_orderby = orderby;
    m_NeedDesc = sortOrder;

    m_ui->tableViewAccountToReceive->sortByColumn(orderby, sortOrder);


}
