/** @file
 *
 * @page accounttypereport Contas
 *
 * Informa o período do relatório, vendas realizadas, contas recebidas, contas pagas e o total. Há informações também de contas a receber, a pagar e o total. 
**/
#include "accounttypereport.h"

#ifdef _OLD_REPORT
#include "dsmreport.h"
#endif

#define SQL_SELECT_SUM_TOPAY "select case when sum(ap.valuepaid) is null then 0 else sum(ap.valuepaid) end as sum "\
"   from fin_accounttype at "\
"   left outer join fin_accounttopay ap on at.id = ap.accounttypeid and ap.paid = true and ap.removed = false and ap.paiddate between '%1' and '%2' "\
"   where (at.type = 0 or at.type = 2) and at.removed = false "

#define SQL_SELECT_SUM_TORECEIVE "select case when sum(ar.valuepaid) is null then 0 else sum(ar.valuepaid) end as sum "\
"   from fin_accounttype at "\
"   left outer join fin_accounttoreceive ar on at.id = ar.accounttypeid and ar.paid = true and ar.removed = false and ar.paiddate between '%1' and '%2' "\
"   where (at.type = 1 or at.type = 2) and at.removed = false "

#define SQL_ACCOUNT_GROUPS_TOPAY "select 	at.description, "\
"   count(ap.id) as quantity, "\
"   to_char(case when sum(ap.value) is null then 0 else sum(ap.value) end,'FM9G999G990D00') as value, "\
"   to_char(case when sum(ap.valuepaid) is null then 0 else sum(ap.valuepaid) end,'FM9G999G990D00') as valuepaid, "\
"   to_char(case when sum(ap.valuepaid - ap.value) is null then 0 else sum(ap.valuepaid - ap.value) end,'FM9G999G990D00') as juros, "\
"   to_char(case when sum(ap.valuepaid) is null then 0 else (sum(ap.valuepaid)/%3) * 100 end,'FM9G999G990D00') as porc "\
"   from fin_accounttype at "\
"   left outer join fin_accounttopay ap on at.id = ap.accounttypeid and ap.paid = true and ap.removed = false and ap.paiddate between '%1' and '%2' "\
"   where (at.type = 0 or at.type = 2) and at.removed = false  "\
"   group by at.description "\
"   order by at.description"

#define SQL_ACCOUNT_GROUPS_TORECEIVE "select 	at.description, "\
"   count(ar.id) as quantity, "\
"   to_char(case when sum(ar.value) is null then 0 else sum(ar.value) end,'FM9G999G990D00') as value, "\
"   to_char(case when sum(ar.valuepaid) is null then 0 else sum(ar.valuepaid) end,'FM9G999G990D00') as valuepaid, "\
"   to_char(case when sum(ar.valuepaid - ar.value) is null then 0 else sum(ar.valuepaid - ar.value) end,'FM9G999G990D00') as juros, "\
"   to_char(case when sum(ar.valuepaid) is null then 0 else (sum(ar.valuepaid)/%3) * 100 end,'FM9G999G990D00') as porc "\
"   from fin_accounttype at "\
"   left outer join fin_accounttoreceive ar on at.id = ar.accounttypeid and ar.paid = true and ar.removed = false and ar.paiddate between '%1' and '%2' "\
"   where (at.type = 1 or at.type = 2) and at.removed = false  "\
"   group by at.description "\
"   order by at.description"

//#define SQL_SELECT_SELLS    "select sum(ps.value) as sum from dsm_sale s inner join debt ps on ps.saleid = s.id and s.status = 1 and ps.paymentwayid <> %3 and ps.paymentwayid <> %4 and ps.paymentwayid <> %5 and s.date between '%1' and '%2'"

/**
 * Relatório de contas.
 *
 * @ingroup report
**/
AccountTypeReport::AccountTypeReport(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountTypeReport)
{
    m_ui->setupUi(this);

    m_ui->dateEditSince->setDate( QDate::currentDate().addMonths(-1) );
    m_ui->dateEditUntil->setDate( QDate::currentDate() );
}

AccountTypeReport::~AccountTypeReport()
{
    delete m_ui;
}

void AccountTypeReport::changeEvent(QEvent *e)
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

void AccountTypeReport::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    m_ui->dateEditSince->setFocus();
}

void AccountTypeReport::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
        case Qt::Key_F1:
        {
            Launcher();
        }
        break;
        case Qt::Key_Escape:
        {
            this->close();
        }
        break;
        default:
        {
            QWidget::keyPressEvent(e);
        }
    }//switch
}

void AccountTypeReport::Launcher(void)
{

#ifdef _OLD_REPORT
    QSqlQueryModel  *getSumToPay = new QSqlQueryModel,
                    *getSumToReceive = new QSqlQueryModel,
                    *getTotalToPay = new QSqlQueryModel,
                    *getTotalToReceive = new QSqlQueryModel;
//                    *getSells = new QSqlQueryModel;

    DSMReport       *report = new DSMReport();
    int             totalToPayAmount = 0, totalToReceiveAmount = 0,
                    totalSells = 0, totalToPay = 0, totalToReceive = 0,
                    totalToPayValue = 0, totalToPayJuros = 0, totalToPayPorc = 0,
                    totalToReceiveValue = 0, totalToReceiveJuros = 0, totalToReceivePorc = 0;

    if ( !report->load( "accounttype" ) )
    {
       QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo modelo." );
       delete report;
       delete getSumToPay;
       delete getSumToReceive;
       delete getTotalToPay;
       delete getTotalToReceive;
//       delete getSells;
       return;
    }

    /** Sells */

  // getSells->setQuery(QString(SQL_SELECT_SELLS)
  //                     .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
  //                     .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB))
  //                     .arg(QRadConfig::GetAccountId())
  //                     .arg(QRadConfig::GetAgreementId())
  //                     .arg(QRadConfig::GetPopDrugId()));
  //

   // totalSells = DSMMoney::StrToInt(getSells->record(0).value("sum").toString());

    /** To Pay */

    getSumToPay->setQuery(QString(SQL_SELECT_SUM_TOPAY)
                     .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                     .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB)));

    getTotalToPay->setQuery(QString(SQL_ACCOUNT_GROUPS_TOPAY)
                       .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                       .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB))
                       .arg(getSumToPay->record(0).value("sum").toDouble()));

    for (int index = 0; index < getTotalToPay->rowCount(); index++)
    {
        totalToPayAmount    += getTotalToPay->record(index).value("quantity").toInt();
        totalToPayValue     += DSMMoney::StrToInt(getTotalToPay->record(index).value("value").toString());
        totalToPay          += DSMMoney::StrToInt(getTotalToPay->record(index).value("valuepaid").toString());
        totalToPayJuros     += DSMMoney::StrToInt(getTotalToPay->record(index).value("juros").toString());
        totalToPayPorc      += DSMMoney::StrToInt(getTotalToPay->record(index).value("porc").toString());
    }

    /** To Receive */

    getSumToReceive->setQuery(QString(SQL_SELECT_SUM_TORECEIVE)
                     .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                     .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB)));

    getTotalToReceive->setQuery(QString(SQL_ACCOUNT_GROUPS_TORECEIVE)
                       .arg(m_ui->dateEditSince->date().toString(FMT_DATE_DB))
                       .arg(m_ui->dateEditUntil->date().toString(FMT_DATE_DB))
                       .arg(getSumToReceive->record(0).value("sum").toDouble()));

    for (int index = 0; index < getTotalToReceive->rowCount(); index++)
    {
        totalToReceiveAmount    += getTotalToReceive->record(index).value("quantity").toInt();
        totalToReceiveValue     += DSMMoney::StrToInt(getTotalToReceive->record(index).value("value").toString());
        totalToReceive          += DSMMoney::StrToInt(getTotalToReceive->record(index).value("valuepaid").toString());
        totalToReceiveJuros     += DSMMoney::StrToInt(getTotalToReceive->record(index).value("juros").toString());
        totalToReceivePorc      += DSMMoney::StrToInt(getTotalToReceive->record(index).value("porc").toString());
    }

    report->setQuery("accounttypetopay", getTotalToPay->query().lastQuery());
    report->setQuery("accounttypetoreceive", getTotalToReceive->query().lastQuery());

    report->setAttributeValue("START_DATE", m_ui->dateEditSince->date().toString(FMT_DATE));
    report->setAttributeValue("END_DATE", m_ui->dateEditUntil->date().toString(FMT_DATE));

    report->setAttributeMoneyValue("TOTAL_SELL", /*totalSells*/ 0);

    report->setAttributeValue("TOTAL_TOPAY_AMOUNT", totalToPayAmount);
    report->setAttributeMoneyValue("TOTAL_TOPAY_VALUE", totalToPayValue);
    report->setAttributeMoneyValue("TOTAL_TOPAY_VALUEPAID", totalToPay);
    report->setAttributeMoneyValue("TOTAL_TOPAY_JUROS", totalToPayJuros);
    report->setAttributeMoneyValue("TOTAL_TOPAY_PORC", totalToPayPorc);

    report->setAttributeValue("TOTAL_TORECEIVE_AMOUNT", totalToReceiveAmount);
    report->setAttributeMoneyValue("TOTAL_TORECEIVE_VALUE", totalToReceiveValue);
    report->setAttributeMoneyValue("TOTAL_TORECEIVE_VALUEPAID", totalToReceive);
    report->setAttributeMoneyValue("TOTAL_TORECEIVE_JUROS", totalToReceiveJuros);
    report->setAttributeMoneyValue("TOTAL_TORECEIVE_PORC", totalToReceivePorc);


    double totalAll = /*totalSells + */totalToReceive - totalToPay;

    if ( totalAll < 0 )
    {
        report->replace("2,3,4", "255,0,0");
    }
    else
    {
        report->replace("2,3,4", "0,170,0");
    }

    report->setAttributeMoneyValue("TOTAL", totalAll);

    if ( !report->show() )
    {
        QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
    }

    delete report;
    delete getSumToPay;
    delete getSumToReceive;
    delete getTotalToPay;
    delete getTotalToReceive;
//    delete getSells;

#endif
}
