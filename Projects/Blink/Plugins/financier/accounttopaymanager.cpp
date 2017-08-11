/** @file
 *
 * @page accounrtopayreport Contas a Pagar
 *
 * Informa a descrição da conta, data de lançamento, de vecimento, de quitação, tipo de conta, fornecedor, banco, valor, valor pago e o status.
**/
#include "accounttopaymanager.h"
#include "ui_accounttopaymanager.h"
#include "qradreportmanager.h"
#include "dsmsgmessages.h"
#include "financierdelegates.h"
#include "qradconfig.h"
#include "qraddebug.h"

#define SQL_SELECT_ACCOUNTTOPAY         "select fac.id, %1 fac.description as description, %1 case when s.nome is null then 'NAO INFORMADO' else s.nome end as fornecedor, %1 fac.issuedate as issuedate, %1 fac.vencdate as vencdate, %1 case when fac.paiddate is null then '2000-01-01' else fac.paiddate end as paiddate, %1 fac.value as value, %1 case when fac.valuepaid is null then 0 else fac.valuepaid end as valuepaid, %1 case when fac.paid is true then 'T' else 'F' end as paid, fac.accounttypeid, fac.supplierid, fac.bankid from fin_accounttopay fac left outer join supplier s on fac.supplierid = s.id where fac.removed = false %2 order by %3, fac.description"
#define SQL_SELECT_ACCOUNTTOPAY_REPORT  "select fac.id, fac.description, to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate, to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate, case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate, to_char(fac.value, 'FM9G999G990D00') as value, to_char(fac.valuepaid, 'FM9G999G990D00') as valuepaid, case when fac.paid = true then 'PAGO' else 'EM ABERTO' end as status, fat.description as accounttype, s.nome as supplier, fb.description as bank from fin_accounttopay fac inner join fin_accounttype fat on fat.id = fac.accounttypeid left outer join supplier s on fac.supplierid = s.id left outer join fin_bank fb on fac.bankid = fb.id where fac.removed = false %1 order by %2, fac.description"
#define SQL_DELETE_ACCOUNTTOPAY         "update fin_accounttopay set removed = true where id = %1"

#define SQL_SELECT_FORMATED             "case when fac.paid = true then 'P' else case when vencdate > current_date then 'T' else case when vencdate < current_date then 'V' else 'H' end end end || "

#define CHECK_STR(str) {str += " and ";}

/**
 * Relatório de conta a pagar
 *
 * @ingroup report
**/
AccountToPayManager::AccountToPayManager(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountToPayManager)
{
    m_ui->setupUi(this);


    m_selectAccountToPay = new QSqlQueryModel;
    m_modelAccountType   = new QSqlQueryModel;
    m_modelSupplier      = new QSqlQueryModel;
    m_modelBank          = new QSqlQueryModel;

    m_ui->dateEditStart->setDate(QDate::currentDate().addDays(-7));
    m_ui->dateEditEnd->setDate(QDate::currentDate().addDays(7));
    m_ui->radioButtonIssueDate->setChecked(true);

    m_ui->checkBoxAccountOpen->setChecked(true);
    m_ui->checkBoxAccountPaid->setChecked(true);

    m_ui->groupBoxAccountType->setChecked(false);
    m_ui->groupBoxSupplier->setChecked(false);

//    InitialConfig();
    GetAccountTypeValues();
    GetSupplierValues();
    GetBankValues();
    InitialConfig();

    m_ui->dateEditStart->setFocus();


    connect(m_ui->btnNew, SIGNAL(pressed()), this, SLOT(NewAccountToPay()));
    connect(m_ui->btnEdit, SIGNAL(pressed()), this, SLOT(EditAccountToPay()));
    connect(m_ui->btnPay, SIGNAL(pressed()), this, SLOT(PayAccount()));
    connect(m_ui->btnDel, SIGNAL(pressed()), this, SLOT(DeleteAccountToPay()));
    connect(m_ui->btnExit, SIGNAL(pressed()), this, SLOT(close()));
    connect(m_ui->btnReport, SIGNAL(pressed()), this, SLOT(ShowReport()));
    connect(m_ui->dateEditStart, SIGNAL(dateChanged(QDate)), this, SLOT(GetAccountToPay()));
    connect(m_ui->dateEditEnd, SIGNAL(dateChanged(QDate)), this, SLOT(GetAccountToPay()));
    connect(m_ui->radioButtonIssueDate, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->radioButtonVencDate, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->radioButtonPaidDate, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->checkBoxAccountOpen, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->checkBoxAccountPaid, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->groupBoxDate, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->groupBoxAccountType, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->groupBoxSupplier, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->groupBoxBank, SIGNAL(clicked()), this, SLOT(GetAccountToPay()));
    connect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));
    connect(m_ui->comboBoxSupplierFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));
    connect(m_ui->comboBoxBankFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));
    connect(m_ui->tableViewAccountToPay->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),
            this, SLOT(fillTheFields(QModelIndex)));
    connect(m_ui->tableViewAccountToPay, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(EditAccountToPay()));


    m_ui->tableViewAccountToPay->setStyleSheet("");
}

AccountToPayManager::~AccountToPayManager()
{
    delete m_selectAccountToPay;
    delete m_ui;
    delete m_modelAccountType;
    delete m_modelSupplier;
    delete m_modelBank;
}

void AccountToPayManager::changeEvent(QEvent *e)
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

void AccountToPayManager::showEvent(QShowEvent *event)
{
    Q_UNUSED(event);

/*
    GetAccountTypeValues();
    GetSupplierValues();
    GetBankValues();
    InitialConfig();

    m_ui->dateEditStart->setFocus();
*/
}

void AccountToPayManager::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ConfigHeaderTable();
}

void AccountToPayManager::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, MSG_INFO_TITLE, MSG_QUESTION_CLOSE, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    {
        event->ignore();
    }
}

void AccountToPayManager::GetAccountToPay(void)
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

        disconnect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));

        GET_COMBOBOX_ID(accountTypeId, m_ui->comboBoxAccountTypeFilter);
        m_strAux += QString(" fac.accounttypeid = %1 ").arg(accountTypeId);

        connect(m_ui->comboBoxAccountTypeFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));
    }

    if (m_ui->groupBoxSupplier->isChecked())
    {
        int supplierId;

        CHECK_STR(m_strAux);

        disconnect(m_ui->comboBoxSupplierFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));

        GET_COMBOBOX_ID(supplierId, m_ui->comboBoxSupplierFilter);
        m_strAux += QString(" fac.supplierid = %1 ").arg(supplierId);

        connect(m_ui->comboBoxSupplierFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));
    }

    if (m_ui->groupBoxBank->isChecked())
    {
        int bankId;

        CHECK_STR(m_strAux);

        disconnect(m_ui->comboBoxBankFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));

        GET_COMBOBOX_ID(bankId, m_ui->comboBoxBankFilter);
        m_strAux += QString(" fac.bankid = %1 ").arg(bankId);

        connect(m_ui->comboBoxBankFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(GetAccountToPay()));
    }

    m_selectAccountToPay->setQuery(QString(SQL_SELECT_ACCOUNTTOPAY)
                                   .arg(SQL_SELECT_FORMATED)
                                   .arg(m_strAux)
                                   .arg(m_dateStr));

    QString strDebug = QString(SQL_SELECT_ACCOUNTTOPAY)
            .arg(SQL_SELECT_FORMATED)
            .arg(m_strAux)
            .arg(m_dateStr);

    debug_message("\nSQL_SELECT_ACCOUNTTOPAY=%s\n", strDebug.toLatin1().data());

    m_ui->tableViewAccountToPay->setModel(m_selectAccountToPay);
  //  m_ui->tableViewAccountToPay->show();
    m_ui->tableViewAccountToPay->selectRow(0);

    double total = 0;
    double totalpaid = 0;
    for (int index = 0; index < m_selectAccountToPay->rowCount(); index++)
    {
        QString strAux = m_selectAccountToPay->record(index).value("value").toString();
        total     = QRadRound::PowerRound(total) + QRadRound::PowerRound(strAux.remove("H").remove("P").remove("V").remove("T").toFloat());
        strAux = m_selectAccountToPay->record(index).value("valuepaid").toString();
        totalpaid = QRadRound::PowerRound(totalpaid) + QRadRound::PowerRound(strAux.remove("H").remove("P").remove("V").remove("T").toFloat());
    }
//    debug_message( "Total: %02.02f  TotalPago: %02.02f\n", total,totalpaid );
//    debug_message( "TotalN: %02.02f  TotalPagoN: %02.02f\n", totalN,totalpaidN );

    m_ui->labelPagar->setText(QString("Total: %1").arg(QRadMoney::MoneyHumanForm(total)));
    m_ui->labelPago->setText(QString("Total Pago: %1").arg(QRadMoney::MoneyHumanForm(totalpaid)));

    m_ui->labelSaldo->setText(QString("Saldo: %1")
                            .arg(QRadMoney::MoneyHumanForm(QRadRound::PowerRound(totalpaid) - QRadRound::PowerRound(total))));

    if( totalpaid>=total )
        m_ui->labelSaldo->setStyleSheet("color: rgb(0, 122, 0);");
    else
        m_ui->labelSaldo->setStyleSheet("color: rgb(135, 0, 0);");

}

void AccountToPayManager::GetAccountTypeValues(void)
{
    m_modelAccountType->setQuery(QString(SQL_SELECT_ACCOUNTTYPE_COMBO).arg(AccountTypeToPay).arg(AccountTypeBoth));
    m_ui->comboBoxAccountType->setModel(m_modelAccountType);
    m_ui->comboBoxAccountType->setModelColumn(1);

    m_ui->comboBoxAccountTypeFilter->setModel(m_modelAccountType);
    m_ui->comboBoxAccountTypeFilter->setModelColumn(1);
}

void AccountToPayManager::GetSupplierValues(void)
{
    m_modelSupplier->setQuery(SQL_SELECT_SUPPLIER_COMBO);
//    m_ui->comboBoxSupplier->setModel(m_modelSupplier);
//    m_ui->comboBoxSupplier->setModelColumn(1);

    m_ui->comboBoxSupplierFilter->setModel(m_modelSupplier);
    m_ui->comboBoxSupplierFilter->setModelColumn(1);
}

void AccountToPayManager::GetBankValues(void)
{
    m_modelBank->setQuery(SQL_SELECT_BANK_COMBO);
    m_ui->comboBoxBank->setModel(m_modelBank);
    m_ui->comboBoxBank->setModelColumn(1);

    m_ui->comboBoxBankFilter->setModel(m_modelBank);
    m_ui->comboBoxBankFilter->setModelColumn(1);

}

void AccountToPayManager::InitialConfig(int row)
{
    GetAccountToPay();
    QModelIndex currentIndex = m_ui->tableViewAccountToPay->currentIndex();

    ConfigHeaderTable();
    m_ui->tableViewAccountToPay->setCurrentIndex(m_selectAccountToPay->index(0,1));

    if (row != 0)
    {
        m_ui->tableViewAccountToPay->selectRow(row);
    }
    else
    {
        m_ui->tableViewAccountToPay->setCurrentIndex(m_selectAccountToPay->index(currentIndex.row(),1));
    }
}

void AccountToPayManager::ConfigHeaderTable(void)
{
    m_ui->tableViewAccountToPay->hideColumn(0);
    m_ui->tableViewAccountToPay->hideColumn(9);
    m_ui->tableViewAccountToPay->hideColumn(10);
    m_ui->tableViewAccountToPay->hideColumn(11);

    m_selectAccountToPay->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Descrição"));
    m_selectAccountToPay->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Fornecedor"));
    m_selectAccountToPay->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Data de\nLançamento"));
    m_selectAccountToPay->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Data de\nVencimento"));
    m_selectAccountToPay->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Data de\nQuitação"));
    m_selectAccountToPay->setHeaderData(6, Qt::Horizontal, QString::fromUtf8("Valor"));
    m_selectAccountToPay->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Valor Pago"));
    m_selectAccountToPay->setHeaderData(8, Qt::Horizontal, QString::fromUtf8("Status"));

    m_ui->tableViewAccountToPay->setItemDelegateForColumn(1, new ColumnFinancierDescription);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(2, new ColumnFinancierDescription);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(3, new ColumnFinancierDate);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(4, new ColumnFinancierDate);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(5, new ColumnFinancierDate);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(6, new ColumnFinancierMoney);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(7, new ColumnFinancierMoney);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(8, new ColumnFinancierAccountPaid);

    m_ui->tableViewAccountToPay->setColumnWidth(1, 0.25 * m_ui->tableViewAccountToPay->width());
    m_ui->tableViewAccountToPay->setColumnWidth(2, 0.15 * m_ui->tableViewAccountToPay->width());
    m_ui->tableViewAccountToPay->setColumnWidth(3, 0.11 * m_ui->tableViewAccountToPay->width());
    m_ui->tableViewAccountToPay->setColumnWidth(4, 0.11 * m_ui->tableViewAccountToPay->width());
    m_ui->tableViewAccountToPay->setColumnWidth(5, 0.10 * m_ui->tableViewAccountToPay->width());
    m_ui->tableViewAccountToPay->setColumnWidth(6, 0.12 * m_ui->tableViewAccountToPay->width());
//    m_ui->tableViewAccountToPay->setColumnWidth(7, 0.12 * m_ui->tableViewAccountToPay->width());
    m_ui->tableViewAccountToPay->horizontalHeader()->setStretchLastSection(true);
}

void AccountToPayManager::fillTheFields(QModelIndex currentIndex)
{
    m_accountToPayId = currentIndex.sibling(currentIndex.row(),0).data().toInt();
    m_accountToPayRow = m_ui->tableViewAccountToPay->currentIndex().row();
    m_accountToPayPaid = (currentIndex.sibling(currentIndex.row(),8).data().toString().mid(1) == "T")?true:false;

    if (m_accountToPayPaid)
    {
        m_ui->btnPay->setText(QString::fromUtf8("Es&tornar"));
    }
    else
    {
        m_ui->btnPay->setText(QString::fromUtf8("&Quitar"));
    }

    m_ui->comboBoxAccountType->setModelColumn(0);
//    m_ui->comboBoxSupplier->setModelColumn(0);
    m_ui->comboBoxBank->setModelColumn(0);

    m_ui->comboBoxAccountType->setCurrentIndex(m_ui->comboBoxAccountType->findText(QString("%1").arg(currentIndex.sibling(currentIndex.row(),9).data().toInt())));
//    m_ui->comboBoxSupplier->setCurrentIndex(m_ui->comboBoxSupplier->findText(QString("%1").arg(currentIndex.sibling(currentIndex.row(),10).data().toInt())));
    m_ui->comboBoxBank->setCurrentIndex(m_ui->comboBoxBank->findText(QString("%1").arg(currentIndex.sibling(currentIndex.row(),11).data().toInt())));

    m_ui->comboBoxAccountType->setModelColumn(1);
//    m_ui->comboBoxSupplier->setModelColumn(1);
    m_ui->comboBoxBank->setModelColumn(1);
}

void AccountToPayManager::keyPressEvent(QKeyEvent *event)
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

void AccountToPayManager::NewAccountToPay(void)
{
    AccountToPayRegister *accountToPayRegister = new AccountToPayRegister(this);
    QRadConfig::centralizarWidget(accountToPayRegister);

    if (accountToPayRegister->exec() == QDialog::Accepted)
    {
        AccountToPayModel *accountToPayModel = new AccountToPayModel;
        AccountToPayHistoryModel *accountToPayHistoryModel = new AccountToPayHistoryModel;

        accountToPayHistoryModel->setAccountToPayId(accountToPayModel->lastInsertId());
        accountToPayHistoryModel->setTypeOperation(AccountOpCreate);
        accountToPayHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
        accountToPayHistoryModel->setDate(QDate::currentDate());
        accountToPayHistoryModel->setTime(QTime::currentTime());

        accountToPayHistoryModel->Create();

        delete accountToPayModel;
        delete accountToPayHistoryModel;
    }
    InitialConfig(m_accountToPayRow);

    delete accountToPayRegister;
}

void AccountToPayManager::EditAccountToPay(void)
{
    if (m_selectAccountToPay->rowCount() > 0)
    {
        AccountToPayRegister *accountToPayRegister = new AccountToPayRegister(this);
        QRadConfig::centralizarWidget(accountToPayRegister);

        accountToPayRegister->SendAccountToPayId(m_accountToPayId);

        if (accountToPayRegister->exec() == QDialog::Accepted)
        {
            AccountToPayHistoryModel *accountToPayHistoryModel = new AccountToPayHistoryModel;

            accountToPayHistoryModel->setAccountToPayId(m_accountToPayId);
            accountToPayHistoryModel->setTypeOperation(AccountOpEdit);
            accountToPayHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
            accountToPayHistoryModel->setDate(QDate::currentDate());
            accountToPayHistoryModel->setTime(QTime::currentTime());

            accountToPayHistoryModel->Create();

            delete accountToPayHistoryModel;
        }
        InitialConfig(m_accountToPayRow);

        delete accountToPayRegister;
    }
}

void AccountToPayManager::PayAccount(void)
{

    debug_message("-->\n");
    if (m_selectAccountToPay->rowCount() > 0)
    {
        if (m_accountToPayPaid)
        {
            if (QMessageBox::question(this, MSG_QUESTION_TITLE, MSG_QUESTION_RESTORE_ACCOUNT, QMessageBox::Yes, QMessageBox::No) == QMessageBox::Yes)
            {
                AccountToPayModel *accountToPayModel = new AccountToPayModel;

                accountToPayModel->setId(m_accountToPayId);

                accountToPayModel->updatePaid(false);
                accountToPayModel->updateValuePaid(0);
                accountToPayModel->updatePaidDate(QDate(2000,1,1));

                delete accountToPayModel;

                AccountToPayHistoryModel *accountToPayHistoryModel = new AccountToPayHistoryModel;

                accountToPayHistoryModel->setAccountToPayId(m_accountToPayId);
                accountToPayHistoryModel->setTypeOperation(AccountOpRestore);
                accountToPayHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
                accountToPayHistoryModel->setDate(QDate::currentDate());
                accountToPayHistoryModel->setTime(QTime::currentTime());

                accountToPayHistoryModel->Create();

                delete accountToPayHistoryModel;

                QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_RECORD_OK);
                InitialConfig(m_accountToPayRow);
            }
        }
        else
        {
            PaidAccount *paidAccount = new PaidAccount(this);
            QRadConfig::centralizarWidget(paidAccount);

            paidAccount->SendPaidAccountId(m_accountToPayId, AccountTypeToPay);

            debug_message("-->paidAccount->exec()\n");
            if (paidAccount->exec() == QDialog::Accepted)
            {
                AccountToPayHistoryModel *accountToPayHistoryModel = new AccountToPayHistoryModel;

                accountToPayHistoryModel->setAccountToPayId(m_accountToPayId);
                accountToPayHistoryModel->setTypeOperation(AccountOpPaid);
                accountToPayHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
                accountToPayHistoryModel->setDate(QDate::currentDate());
                accountToPayHistoryModel->setTime(QTime::currentTime());

                debug_message("-->accountToPayHistoryModel->Create()\n");

                accountToPayHistoryModel->Create();

                debug_message("-->delete accountToPayHistoryModel\n");
                delete accountToPayHistoryModel;
            }
            debug_message("-->InitialConfig(m_accountToPayRow)\n");
            InitialConfig(m_accountToPayRow);

            debug_message("-->delete paidAccount\n");
            delete paidAccount;
        }
    }
    debug_message("<--\n");
}

void AccountToPayManager::DeleteAccountToPay(void)
{
    if (m_selectAccountToPay->rowCount() > 0)
    {
        if (m_accountToPayId)
        {
            QModelIndex index = m_ui->tableViewAccountToPay->currentIndex();

            QString strText = QString("%1 (%2)").arg(MSG_QUESTION_DELETE_ACCOUNTTOPAY)
                    .arg( m_selectAccountToPay->record(m_ui->tableViewAccountToPay->currentIndex().row()).value("description").toString() );

            if (QMessageBox::question(this, MSG_QUESTION_TITLE, strText, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
            {
                AccountToPayModel *accountToPayModel = new AccountToPayModel;

                accountToPayModel->setId(m_accountToPayId);
                accountToPayModel->updateRemoved(true);

                delete accountToPayModel;

                AccountToPayHistoryModel *accountToPayHistoryModel = new AccountToPayHistoryModel;

                accountToPayHistoryModel->setAccountToPayId(m_accountToPayId);
                accountToPayHistoryModel->setTypeOperation(AccountOpRemove);
                accountToPayHistoryModel->setUserId(QRadConfig::GetCurrentUserId());
                accountToPayHistoryModel->setDate(QDate::currentDate());
                accountToPayHistoryModel->setTime(QTime::currentTime());

                accountToPayHistoryModel->Create();

                delete accountToPayHistoryModel;

                QMessageBox::information(this, MSG_INFO_TITLE, MSG_INFO_DELETE_OK);
                InitialConfig(m_accountToPayRow);
            }
        }
    }
}

void AccountToPayManager::ShowReport(void)
{
    if (m_selectAccountToPay->rowCount() > 0)
    {
        QSqlQueryModel *select = new QSqlQueryModel;
        QRadReportManager   *report = new QRadReportManager();
        int total = 0, totalPaid = 0;

        if ( !report->load( "accounttopay" ) )
        {
           QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo modelo." );
           delete report;
           return;
        }

        select->setQuery(QString(SQL_SELECT_ACCOUNTTOPAY_REPORT)
                         .arg(m_strAux)
                         .arg(m_dateStr));

        for (int index = 0; index < select->rowCount(); index++)
        {
            total       += QRadMoney::StrToInt(select->record(index).value("value").toString());
            totalPaid   += QRadMoney::StrToInt(select->record(index).value("valuepaid").toString());
        }


        report->setQuery("account", QString(SQL_SELECT_ACCOUNTTOPAY_REPORT)
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

