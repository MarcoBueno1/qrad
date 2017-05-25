#include "accountcalendaraux.h"
#include "column2delegate.h"


#define SQL_SELECT_ACCOUNTTOPAY     "select fac.id, fac.description, fac.issuedate, fac.vencdate, fac.value  from dsm_fin_accounttopay fac where fac.removed = false and fac.vencdate = '%1' and fac.paid = false order by fac.description"
#define SQL_SELECT_ACCOUNTTORECEIVE "select fac.id, fac.description, fac.issuedate, fac.vencdate, fac.value  from dsm_fin_accounttoreceive fac where fac.removed = false and fac.vencdate = '%1' and fac.paid = false order by fac.description"

AccountCalendarAux::AccountCalendarAux(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountCalendarAux)
{
    m_ui->setupUi(this);

    m_accountToPayModel = new QSqlQueryModel;
    m_accountToReceiveModel = new QSqlQueryModel;
}

AccountCalendarAux::~AccountCalendarAux()
{
    delete m_accountToPayModel;
    delete m_accountToReceiveModel;
    delete m_ui;
}

void AccountCalendarAux::changeEvent(QEvent *e)
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

void AccountCalendarAux::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    InitialConfig();
    ConfigHeaderTable();
}

void AccountCalendarAux::SendDate(QDate date)
{
    m_date = date;
}

void AccountCalendarAux::InitialConfig()
{
    m_accountToPayModel->setQuery(QString(SQL_SELECT_ACCOUNTTOPAY).arg(m_date.toString(FMT_DATE_DB)));
    m_ui->tableViewAccountToPay->setModel(m_accountToPayModel);


    m_accountToReceiveModel->setQuery(QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(m_date.toString(FMT_DATE_DB)));
    m_ui->tableViewAccountToReceive->setModel(m_accountToReceiveModel);

}

void AccountCalendarAux::ConfigHeaderTable(void)
{
    m_ui->tableViewAccountToPay->hideColumn(0);

    m_accountToPayModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Descrição"));
    m_accountToPayModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Data de\nEmissão"));
    m_accountToPayModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Data de\nVencimento"));
    m_accountToPayModel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Valor"));

    m_ui->tableViewAccountToPay->setItemDelegateForColumn(2, new ColumnDate);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(3, new ColumnDate);
    m_ui->tableViewAccountToPay->setItemDelegateForColumn(4, new ColumnMoney);

    m_ui->tableViewAccountToPay->setColumnWidth(1, 240);
    m_ui->tableViewAccountToPay->setColumnWidth(2, 80);
    m_ui->tableViewAccountToPay->setColumnWidth(3, 80);
    m_ui->tableViewAccountToPay->setColumnWidth(4, 90);

    m_ui->tableViewAccountToReceive->hideColumn(0);

    m_accountToReceiveModel->setHeaderData(1, Qt::Horizontal, QString::fromUtf8("Descrição"));
    m_accountToReceiveModel->setHeaderData(2, Qt::Horizontal, QString::fromUtf8("Data de\nEmissão"));
    m_accountToReceiveModel->setHeaderData(3, Qt::Horizontal, QString::fromUtf8("Data de\nVencimento"));
    m_accountToReceiveModel->setHeaderData(4, Qt::Horizontal, QString::fromUtf8("Valor"));

    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(2, new ColumnDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(3, new ColumnDate);
    m_ui->tableViewAccountToReceive->setItemDelegateForColumn(4, new ColumnMoney);

    m_ui->tableViewAccountToReceive->setColumnWidth(1, 240);
    m_ui->tableViewAccountToReceive->setColumnWidth(2, 80);
    m_ui->tableViewAccountToReceive->setColumnWidth(3, 80);
    m_ui->tableViewAccountToReceive->setColumnWidth(4, 90);
}
