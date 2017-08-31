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

#define SQL_SELECT_ACCOUNTTORECEIVE         "select fac.id, %1 fac.description as description,%1 case when c.name is NULL then 'NAO INFORMADO' else c.name end as cliente, %1 fac.issuedate as issuedate, %1 fac.vencdate as vencdate, %1 case when fac.paiddate is null then '2000-01-01' else fac.paiddate end as paiddate, %1 fac.value as value, %1 case when fac.valuepaid is null then 0 else fac.valuepaid end as valuepaid, %1 case when fac.paid is true then 'T' else 'F' end as paid, fac.accounttypeid, fac.clientid from fin_accounttoreceive fac %4 left join dweller c on c.id = fac.clientid where fac.removed = false %2 order by %3, fac.description"
#define SQL_SELECT_ACCOUNTTORECEIVE_REPORT  "select fac.id, fac.description, to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate, to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate, case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate, to_char(fac.value, 'FM9G999G990D00') as value, to_char(fac.valuepaid, 'FM9G999G990D00') as valuepaid, case when fac.paid = true then 'PAGO' else 'EM ABERTO' end as status, fat.description as accounttype, case when c.name is NULL then 'NAO INFORMADO' else c.name end as client from fin_accounttoreceive fac inner join fin_accounttype fat on fat.id = fac.accounttypeid %3 left join dweller c on fac.clientid = c.id where fac.removed = false %1 order by %2, fac.description"
#define SQL_DELETE_ACCOUNTTORECEIVE         "update fin_accounttoreceive set removed = true where id = %1"

#define SQL_SELECT_FORMATED                 "case when fac.paid = true then 'P' else case when vencdate > current_date then 'T' else case when vencdate < current_date then 'V' else 'H' end end end || "

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
    m_strAux = "";
    if (m_ui->radioButtonIssueDate->isChecked())
    {
        m_dateStr = "fac.issuedate";
    }
    else if (m_ui->radioButtonVencDate->isChecked())
    {
        m_dateStr = "fac.vencdate";
    }
    else
    {
        m_dateStr = "fac.paiddate";
    }

    if (m_ui->groupBoxDate->isChecked())
    {
        CHECK_STR(m_strAux);
        m_strAux += QString(" %1 between '%2' and '%3' ")
                                .arg(m_dateStr)
                                .arg(m_ui->dateEditStart->date().toString(FMT_DATE_DB))
                                .arg(m_ui->dateEditEnd->date().toString(FMT_DATE_DB));
    }

    if ( (m_ui->checkBoxAccountOpen->isChecked()) && (m_ui->checkBoxAccountPaid->isChecked()) )
    {
        /** nao adiciona nada */
    }
    else if (m_ui->checkBoxAccountOpen->isChecked())
    {
        CHECK_STR(m_strAux);
        m_strAux += " fac.paid = false ";
    }
    else if (m_ui->checkBoxAccountPaid->isChecked())
    {
        CHECK_STR(m_strAux);
        m_strAux += " fac.paid = true ";
    }
    else
    {
        CHECK_STR(m_strAux);
        m_strAux += " fac.paid = true and fac.paid = false ";
    }

    if (m_ui->groupBoxAccountType->isChecked())
    {
        int accountTypeId;

        CHECK_STR(m_strAux);

        disconnect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));

        GET_COMBOBOX_ID(accountTypeId, m_ui->comboBoxAccountTypeFilter);
        m_strAux += QString(" fac.accounttypeid = %1 ").arg(accountTypeId);

        connect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));
    }

    if (m_ui->groupBoxClient->isChecked())
    {
        int clientId;

        CHECK_STR(m_strAux);

        disconnect(m_ui->comboBoxClientFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));

        GET_COMBOBOX_ID(clientId, m_ui->comboBoxClientFilter);
        m_strAux += QString(" fac.clientid = %1 ").arg(clientId);

        connect(m_ui->comboBoxClientFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToReceive()));
    }

    //// inner join ticket ....
    if( m_ui->radioButtonOlympic->isChecked())
    {
       m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id  ";
       m_InnerJoinTicket +=  " inner join dweller dw on dw.id = tkt.clientid and dw.tower = 1 ";
    }
    else if( m_ui->radioButtonMarine->isChecked())
    {
       m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id  ";
       m_InnerJoinTicket +=  " inner join dweller dw on dw.id = tkt.clientid and dw.tower = 2 ";
    }
    else if( m_ui->radioButtonGreen->isChecked())
    {
       m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id ";
       m_InnerJoinTicket +=  " inner join dweller dw on dw.id = tkt.clientid and dw.tower = 3 ";
    }

    if( m_ui->radioButtonAllType->isChecked())
        m_ui->comboBoxTypeTxExtr->setVisible(false);
    if( m_ui->radioButtonTxCond->isChecked())
    {
        m_ui->comboBoxTypeTxExtr->setVisible(false);
        if(m_InnerJoinTicket.isEmpty())
            m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id and tkt.type = 0 ";
        else
            m_InnerJoinTicket.insert(strlen(" inner join ticket tkt on tkt.accountid = fac.id ")," and tkt.type = 0 ");

    }
    else if( m_ui->radioButtonTxExtra->isChecked())
    {
        QString aux2;
        if(!m_ui->comboBoxTypeTxExtr->isVisible())
        {
            m_ui->comboBoxTypeTxExtr->setVisible(true);
        }
        int currentid = m_ui->comboBoxTypeTxExtr->model()->data(m_ui->comboBoxTypeTxExtr->model()->index(m_ui->comboBoxTypeTxExtr->currentIndex(), 0)).toInt();
        if( currentid > 0 )
        {
            debug_message("Current index of type of extratax: %d ........\n", currentid);
            aux2 = QString(" inner join extratx ext on ext.id = tkt.extratxid and ext.motivo = %1 ").arg(currentid);
        }


        if(m_InnerJoinTicket.isEmpty())
            m_InnerJoinTicket =   " inner join ticket tkt on tkt.accountid = fac.id and tkt.type = 1 " + aux2;
        else
            m_InnerJoinTicket.insert(strlen(" inner join ticket tkt on tkt.accountid = fac.id ")," and tkt.type = 1 " + aux2);
    }



    m_selectAccountToReceive->setQuery(QString(SQL_SELECT_ACCOUNTTORECEIVE)
                                   .arg(SQL_SELECT_FORMATED)
                                   .arg(m_strAux)
                                   .arg(m_dateStr)
                                   .arg(m_InnerJoinTicket));

    QString strDebug = QString(SQL_SELECT_ACCOUNTTORECEIVE)
            .arg(SQL_SELECT_FORMATED)
            .arg(m_strAux)
            .arg(m_dateStr)
            .arg(m_InnerJoinTicket);

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
    m_ui->tableViewAccountToReceive->hideColumn(9);
    m_ui->tableViewAccountToReceive->hideColumn(10);
    m_ui->tableViewAccountToReceive->hideColumn(11);

    m_selectAccountToReceive->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Descrição"));
    m_selectAccountToReceive->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Devedor"));
    m_selectAccountToReceive->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Data de Lançamento"));
    m_selectAccountToReceive->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Data de Vencimento"));
    m_selectAccountToReceive->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Data de Quitação"));
    m_selectAccountToReceive->setHeaderData(6, Qt::Horizontal, QString::fromUtf8("Valor"));
    m_selectAccountToReceive->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Valor Pago"));
    m_selectAccountToReceive->setHeaderData(8, Qt::Horizontal, QString::fromUtf8("Status"));

    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(1, new ColumnFinancierDescription);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(2, new ColumnFinancierDescription);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(3, new ColumnFinancierDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(4, new ColumnFinancierDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(5, new ColumnFinancierDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(6, new ColumnFinancierMoney);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(7, new ColumnFinancierMoney);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(8, new ColumnFinancierAccountPaid);

    m_ui->tableViewAccountToReceive->setColumnWidth(1, 0.25 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->setColumnWidth(2, 0.15 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->setColumnWidth(3, 0.11 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->setColumnWidth(4, 0.11 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->setColumnWidth(5, 0.10 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->setColumnWidth(6, 0.10 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->setColumnWidth(7, 0.10 * m_ui->tableViewAccountToReceive->width());
    m_ui->tableViewAccountToReceive->horizontalHeader()->setStretchLastSection(true);
}

void AccountToReceiveManager::fillTheFields(QModelIndex currentIndex)
{
    m_accountToReceiveId = currentIndex.sibling(currentIndex.row(),0).data().toInt();
    m_accountToReceiveRow = m_ui->tableViewAccountToReceive->currentIndex().row();
    m_accountToReceivePaid = (currentIndex.sibling(currentIndex.row(),8).data().toString().mid(1) == "T")?true:false;

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

            paidAccount->SendPaidAccountId(m_accountToReceiveId, AccountTypeToReceive);

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
    if (m_selectAccountToReceive->rowCount() > 0)
    {
        QSqlQueryModel *select = new QSqlQueryModel;
        QRadReportManager   *report = new QRadReportManager();
        int total = 0, totalPaid = 0;

        if ( !report->load( "accounttoreceive" ) )
        {
           QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo modelo." );
           delete report;
           return;
        }

        select->setQuery(QString(SQL_SELECT_ACCOUNTTORECEIVE_REPORT)
                         .arg(m_strAux)
                         .arg(m_dateStr)
                         .arg(m_InnerJoinTicket));

        debug_message("SQL: %s\n", QString(SQL_SELECT_ACCOUNTTORECEIVE_REPORT)
                      .arg(m_strAux)
                      .arg(m_dateStr).toLatin1().data());


        for (int index = 0; index < select->rowCount(); index++)
        {
            total       += QRadMoney::StrToInt(select->record(index).value("value").toString());
            totalPaid   += QRadMoney::StrToInt(select->record(index).value("valuepaid").toString());
        }

        report->setQuery("account", QString(SQL_SELECT_ACCOUNTTORECEIVE_REPORT)
                                                .arg(m_strAux)
                                                .arg(m_dateStr));

        report->setAttributeMoneyValue("TOTAL", total);
        report->setAttributeMoneyValue("TOTAL_PAID", totalPaid);

        if ( !report->show() )
        {
            QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
        }

        delete report;
        delete select;
    }
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
