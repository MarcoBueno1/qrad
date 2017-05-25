#include "accounthistoryreportaux.h"

#include "column2delegate.h"
#include "financierdelegates.h"


#define SQL_SELECT_ACCOUNTTOPAY     "select ah.id, ah.userid, a.description, 'A PAGAR' as accounttype, ah.typeoperation, u.name, ah.date, ah.time  from fin_accounttopay_history ah inner join fin_accounttopay a on ah.accounttopayid = a.id and a.id = %1 inner join dsm_user u on ah.userid = u.id order by date desc, time desc"
#define SQL_SELECT_ACCOUNTTORECEIVE "select ah.id, ah.userid, a.description, 'A RECEBER' as accounttype, ah.typeoperation, u.name, ah.date, ah.time  from fin_accounttoreceive_history ah inner join fin_accounttoreceive a on ah.accounttoreceiveid = a.id and a.id = %1 inner join dsm_user u on ah.userid = u.id order by date desc, time desc"

AccountHistoryReportAux::AccountHistoryReportAux(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountHistoryReportAux)
{
    m_ui->setupUi(this);

    m_accountModel = new QSqlQueryModel;
}

AccountHistoryReportAux::~AccountHistoryReportAux()
{
    delete m_accountModel;
    delete m_ui;
}

void AccountHistoryReportAux::changeEvent(QEvent *e)
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

void AccountHistoryReportAux::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    InitialConfig();
}

void AccountHistoryReportAux::SendAccountId(int accountId, DSM_AccountType_t accountType)
{
    m_accountId = accountId;
    m_accountType = accountType;
}

void AccountHistoryReportAux::InitialConfig()
{
    if (m_accountType == AccountTypeToPay)
    {
        m_accountModel->setQuery(QString(SQL_SELECT_ACCOUNTTOPAY).arg(m_accountId));
    }
    else
    {
        m_accountModel->setQuery(QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(m_accountId));
    }

    m_ui->tableViewAccountHistory->setModel(m_accountModel);
    m_ui->tableViewAccountHistory->selectRow(0);

    ConfigHeaderTable();

    m_ui->tableViewAccountHistory->selectRow(0);
}

void AccountHistoryReportAux::ConfigHeaderTable(void)
{
    m_ui->tableViewAccountHistory->hideColumn(0);
    m_ui->tableViewAccountHistory->hideColumn(1);

    m_accountModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Descrição"));
    m_accountModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Tipo de\nConta"));
    m_accountModel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Tipo de\nOperação"));
    m_accountModel->setHeaderData(5, Qt::Horizontal, QString::fromUtf8("Responsável"));
    m_accountModel->setHeaderData(6, Qt::Horizontal, QString::fromUtf8("Data"));
    m_accountModel->setHeaderData(7, Qt::Horizontal, QString::fromUtf8("Hora"));

    m_ui->tableViewAccountHistory->setItemDelegateForColumn(2, new ColumnObs);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(3, new ColumnCenter);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(4, new ColumnFinancierAccountOperation);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(5, new ColumnObs);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(6, new ColumnDate);
    m_ui->tableViewAccountHistory->setItemDelegateForColumn(7, new ColumnCenter);

    m_ui->tableViewAccountHistory->setColumnWidth(2, 260);
    m_ui->tableViewAccountHistory->setColumnWidth(3, 90);
    m_ui->tableViewAccountHistory->setColumnWidth(4, 90);
    m_ui->tableViewAccountHistory->setColumnWidth(5, 160);
    m_ui->tableViewAccountHistory->setColumnWidth(6, 80);
    m_ui->tableViewAccountHistory->setColumnWidth(7, 75);
    //m_ui->tableViewAccountHistory->horizontalHeader()->setStretchLastSection(true);
}
