#include "actoreceivereport.h"
#include "ui_actoreceivereport.h"
#include "qradshared.h"
#include "qradreportmanager.h"
#include <QMessageBox>
#include <QCompleter>



#define SQL_PESSOA "select id, name from dweller where payer = true and removed = false order by name"
#define SQL_AP_TORRE "select d.id, ap.numero || ' ' || substring(t.name from 1 for 1) from dweller d inner join ap on ap.id = d.ap inner join tower t on d.tower = t.id where d.payer = true and d.removed = false order by t.name desc, ap.id"


actoReceiveReport::actoReceiveReport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::actoReceiveReport)
{
    ui->setupUi(this);

    m_pessoaAP = new QSqlQueryModel;


    ui->comboBoxTypeTxExtr->setTable("reasonextratx");
    ui->comboBoxTypeTxExtr->setField("description");
    ui->comboBoxTypeTxExtr->setCanAdd(false);
    ui->comboBoxTypeTxExtr->setUserName("dsm");
    if( ui->comboBoxTypeTxExtr->completer() )
        ui->comboBoxTypeTxExtr->completer()->setFilterMode(Qt::MatchContains );

    ui->comboBoxTypeTxExtr->setVisible(false);

    ui->dateEditFrom->setDate(QDate::currentDate());
    ui->dateEditUntil->setDate(QDate::currentDate());

    connect(ui->radioButtonTxExtra,SIGNAL(clicked(bool)),this,SLOT(ManageTxExtra()));
    connect(ui->radioButtonCondominio,SIGNAL(clicked(bool)),this,SLOT(ManageTxExtra()));

    connect(ui->radioButtonPeople,SIGNAL(clicked(bool)),this,SLOT(ManagePessoaAP()));
    connect(ui->radioButtonAPTorre,SIGNAL(clicked(bool)),this,SLOT(ManagePessoaAP()));

    ManagePessoaAP();

    m_AccountType = new QSqlQueryModel;
    m_AccountType->setQuery(SQL_SELECT_ACCOUNTTYPE_COMBO);
    ui->comboBoxTipodeConta->setModel(m_AccountType);
    ui->comboBoxTipodeConta->setModelColumn(1);

    connect(ui->pushButtonBuild,SIGNAL(clicked(bool)),this,SLOT(BuildReport(bool)));
}

actoReceiveReport::~actoReceiveReport()
{
    delete m_pessoaAP;
    delete m_AccountType;
    delete ui;
}
void actoReceiveReport::ManageTxExtra()
{
    if(ui->radioButtonTxExtra->isChecked())
        ui->comboBoxTypeTxExtr->setVisible(true);
    else
        ui->comboBoxTypeTxExtr->setVisible(false);

}
void actoReceiveReport::ManagePessoaAP()
{
    if(ui->radioButtonPeople->isChecked())
    {
        m_pessoaAP->setQuery(SQL_PESSOA);
    }
    else
    {
        m_pessoaAP->setQuery(SQL_AP_TORRE);
    }

    ui->comboBoxPessoaAp->setModel(m_pessoaAP);
    ui->comboBoxPessoaAp->setModelColumn(1);
}

QString actoReceiveReport::MountSQLReport()
{
#define SQL_SELECT_ACCOUNTTORECEIVE "select fac.id, case when a.numero is null then fac.description else a.numero || ' ' || substring(t.name from 1 for 1) end as description, "\
                                    " case when c.name is NULL then 'NAO INFORMADO' else c.name end as client, "\
                                    " to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate, to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate, "\
                                    " case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate, "\
                                    " to_char(fac.value, 'FM9G999G990D00') as value, "\
                                    " case when fac.paid = true then 'PAGO' else 'EM ABERTO' end as status, "\
                                    " case when tkt.type is null then fat.description else case when tkt.type = 0 then 'CONDOMÍNIO' else 'TX EXTRA' end end as accounttype, "\
                                    " case when fac.paid = true then 'P' else case when vencdate > current_date then 'T' else "\
                                    " case when vencdate < current_date then 'V' else 'H' end end end as situation, "\
                                    " case when fac.valuepaid is null then 0 else fac.valuepaid end as valuepaid "\
                                    " from fin_accounttoreceive fac inner join fin_accounttype fat on fat.id = fac.accounttypeid "\
                                    " %1 join ticket tkt on tkt.accountid = fac.id %2 "\
                                    " %1 join dweller c on tkt.clientid = c.id "\
                                    " %1 join ap a on c.ap = a.id "\
                                    " %1 join tower t on c.tower = t.id "\
                                    " where fac.removed = false %3 %4"


    QString aux;
    QString Where = " and ";
    QString InnerJoinExtrTax;
    QString JoinType = " left ";
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
       JoinType  = " inner ";
       if( Where.length() > 5)
            Where += " and ";

       Where += QString(" c.id = %1 ").arg(m_pessoaAP->index(ui->comboBoxPessoaAp->currentIndex(),0).data().toInt());
    }

    if( ui->groupBoxApenasTipo->isChecked() )
    {
        JoinType  = " inner ";

        if( ui->radioButtonCondominio->isChecked() )
        {
            if( Where.length() > 5)
                 Where += " and ";
            Where += QString(" tkt.type = %1 ").arg(0);
        }
        else
        {
            int currentid = ui->comboBoxTypeTxExtr->model()->data(ui->comboBoxTypeTxExtr->model()->index(ui->comboBoxTypeTxExtr->currentIndex(), 0)).toInt();
            if( currentid > 0 )
            {
                InnerJoinExtrTax = QString(" inner join extratx ext on ext.id = tkt.extratxid and ext.motivo = %1 ").arg(currentid);
            }
        }
    }
    if( ui->groupBoxFilterTower->isChecked() )
    {
        JoinType  = " inner ";

        if( Where.length() > 5)
             Where += " and ";

        if( ui->radioButtonOlympic->isChecked() )
        {
            Where += " t.id = 1 ";
        }
        else if( ui->radioButtonMarine->isChecked())
        {
            Where += " t.id = 2 ";
        }
        else
        {
            Where += " t.id = 3 ";
        }
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
    }

    /// order by
    if( ui->checkBoxOrderApTorre->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy = " order by ";

        OrderBy += " t.name desc, a.id";
    }
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

        OrderBy += " c.name";
    }
    if( ui->checkBoxOrderSituation->isChecked() )
    {
        if( !OrderBy.length() )
            OrderBy =  " order by ";
        if( OrderBy != " order by ")
            OrderBy += ",";

        OrderBy += " fac.paid";
    }

    if(Where.length() == 5)
        Where = "";

    aux = QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(JoinType).arg(InnerJoinExtrTax).arg(Where).arg(OrderBy);
    return aux;
}

void actoReceiveReport::BuildReport(bool)
{

    this->setEnabled(false);
  QRadReportManager   *report = new QRadReportManager();

  if ( !report->load( "accounttoreceive" ) )
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
