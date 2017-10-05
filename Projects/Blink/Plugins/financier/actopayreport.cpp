#include "actopayreport.h"
#include "ui_actopayreport.h"
#include "qradshared.h"
#include "qradreportmanager.h"
#include <QMessageBox>
#include "qraddebug.h"



actopayreport::actopayreport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::actopayreport)
{
    ui->setupUi(this);

    ui->dateEditFrom->setDate(QDate::currentDate());
    ui->dateEditUntil->setDate(QDate::currentDate());

    m_Fornecedor = new QSqlQueryModel;
    m_Fornecedor->setQuery(SQL_SELECT_SUPPLIER_COMBO);
    ui->comboBoxFornecedor->setModel(m_Fornecedor);
    ui->comboBoxFornecedor->setModelColumn(1);

    m_AccountType = new QSqlQueryModel;
    m_AccountType->setQuery(SQL_SELECT_ACCOUNTTYPE_COMBO);
    ui->comboBoxTipodeConta->setModel(m_AccountType);
    ui->comboBoxTipodeConta->setModelColumn(1);

    connect(ui->pushButtonBuild,SIGNAL(clicked(bool)),this,SLOT(BuildReport(bool)));
}

actopayreport::~actopayreport()
{
    delete m_Fornecedor;
    delete ui;
}
QString actopayreport::MountSQLReport()
{
#define SQL_SELECT_ACCOUNTTOPAY_REPORT  " select fac.id, "\
                                        " fac.description, "\
                                        " to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate, "\
                                        " to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate, "\
                                        " case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate, "\
                                        " to_char(fac.value, 'FM9G999G990D00') as value, "\
                                        " to_char(fac.valuepaid, 'FM9G999G990D00') as valuepaid, "\
                                        " case when fac.paid = true then 'PAGO' when current_date > fac.vencdate then 'VENCIDA' else 'A VENCER' end as status, "\
                                        " fat.description as accounttype, "\
                                        " s.nome as supplier, "\
                                        " fb.description as bank "\
                                        " from fin_accounttopay fac "\
                                        " inner join fin_accounttype fat on fat.id = fac.accounttypeid "\
                                        " left outer join supplier s on fac.supplierid = s.id "\
                                        " left outer join fin_bank fb on fac.bankid = fb.id "\
                                        " where fac.removed = false %1 %2 "

    QString aux;
    QString Where = " and ";
    QString WhereVencidas;
    QString OrderBy;
    if( ui->groupBoxFilterData->isChecked() )
    {
        if (ui->radioButtonFilterIssue->isChecked())
        {
            aux = "fac.issuedate";
        }
        else if (ui->radioButtonFilterVencto->isChecked())
        {
            aux = "fac.vencdate";
        }
        else
        {
            aux = "fac.paiddate";
        }

        Where += QString(" %1 between '%2' and '%3' ")
                                .arg(aux)
                                .arg(ui->dateEditFrom->date().toString(FMT_DATE_DB))
                                .arg(ui->dateEditUntil->date().toString(FMT_DATE_DB));
    }

    if( ui->groupBoxFilterApenasDe->isChecked() )
    {
        if( Where.length() > 5)
            Where += " and ";

       Where += QString(" s.id = %1 ").arg(m_Fornecedor->index(ui->comboBoxFornecedor->currentIndex(),0).data().toInt());
       WhereVencidas += QString(" and  s.id = %1 ").arg(m_Fornecedor->index(ui->comboBoxFornecedor->currentIndex(),0).data().toInt());
    }

    //// p
    if( ui->checkBoxPaid->isChecked() && ui->checkBoxNotPaid->isChecked())
    {
        /// nothing to do
    }
    else if (ui->checkBoxPaid->isChecked())
    {
        if( Where.length() > 5)
             Where += " and ";

        Where += " fac.paid = true ";
    }
    else
    {
        if( Where.length() > 5)
             Where += " and ";

        Where += " fac.paid = false ";
    }

    if( ui->groupBoxTipoDeConta->isChecked() )
    {
        int accountTypeId;

        GET_COMBOBOX_ID(accountTypeId, ui->comboBoxTipodeConta);

        if( Where.length() > 5)
             Where += " and ";

        Where += QString(" fac.accounttypeid = %1 ").arg(accountTypeId);
        WhereVencidas += QString(" and fac.accounttypeid = %1 ").arg(accountTypeId);
    }

    /// order by
    if( ui->checkBoxOrderIssue->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " fac.issuedate";
    }
    if( ui->checkBoxOrderVncto->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " fac.vencdate";
    }
    if( ui->checkBoxOrderPayment->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " fac.paiddate";
    }
    if( ui->checkBoxOrderPessoa->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " s.name";
    }
    if( ui->checkBoxOrderSituation->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " fac.paid";
    }

    if( ui->checkBoxOrderTipo->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " fat.description";

    }



    if( ui->checkBoxVencidas->isChecked() )
    {
        QString WhereUnion = QString(" and ( fac.paid = false and fac.vencdate < '%1') %2 ")
                .arg(QDate::currentDate().toString(FMT_DATE_DB))
                .arg(WhereVencidas);
        QString Union = "(" + QString(SQL_SELECT_ACCOUNTTOPAY_REPORT).arg(WhereUnion).arg(OrderBy);
        aux  =  QString(" %1) union  ( ")
                .arg(Union);
        aux += QString(SQL_SELECT_ACCOUNTTOPAY_REPORT).arg(Where).arg(OrderBy);
        aux += ") ";
        aux += OrderBy.replace("fac.paid", "status").remove("fac.").replace("s.name", "supplier" ).replace("fat.description", "accounttype");
    }
    else
    {
        aux = QString(SQL_SELECT_ACCOUNTTOPAY_REPORT).arg(Where).arg(OrderBy);

    }
    return aux;
}
void actopayreport::BuildReport(bool)
{

  this->setEnabled(false);
  QRadReportManager   *report = new QRadReportManager();

  if ( !report->load( "accounttopay" ) )
  {
     QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo modelo." );
     delete report;
     this->setEnabled(true);
     return;
  }


  QSqlQueryModel *select = new QSqlQueryModel;
  int total = 0, totalPaid = 0;

  QString SQLReport = MountSQLReport();

  select->setQuery(SQLReport);


  debug_message("SQL Report:%s\n",SQLReport.toLatin1().data());

  for (int index = 0; index < select->rowCount(); index++)
  {
      total       += QRadMoney::StrToInt(select->record(index).value("value").toString());
      totalPaid   += QRadMoney::StrToInt(select->record(index).value("valuepaid").toString());
  }

  report->setQuery("account", SQLReport);

  report->setAttributeMoneyValue("TOTAL", total);
  report->setAttributeMoneyValue("TOTAL_PAID", totalPaid);

  if ( !report->show() )
  {
      QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
  }

  delete report;
  delete select;
  this->setEnabled(true);

}
