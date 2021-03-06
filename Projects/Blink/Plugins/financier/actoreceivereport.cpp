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

    connect(ui->radioButtonFilterPayment,SIGNAL(clicked(bool)),this,SLOT(EnableDisableVencidas()));
    connect(ui->radioButtonFilterIssue,SIGNAL(clicked(bool)),this,SLOT(EnableDisableVencidas()));
    connect(ui->radioButtonFilterVencto,SIGNAL(clicked(bool)),this,SLOT(EnableDisableVencidas()));

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
//" to_char(fac.value, 'FM9G999G990D00') as value, "

QString actoReceiveReport::MountSQLReport()
{
#define SQL_SELECT_ACCOUNTTORECEIVE "select fac.id, case when a.numero is null then fac.description else a.numero || ' ' || substring(t.name from 1 for 1) end as description, "\
                                    " case when c.name is NULL then 'NAO INFORMADO' else c.name end as client, "\
                                    " to_char(fac.issuedate, 'dd-mm-yyyy') as issuedate, to_char(fac.vencdate, 'dd-mm-yyyy') as vencdate, "\
                                    " case when fac.paiddate = '2000-01-01' then '-' else to_char(fac.paiddate, 'dd-mm-yyyy') end as paiddate, "\
                                    " to_char(fac.value, 'FM9G999G990D00') as originalvalue, "\
                                    " case when paid <> true and current_date > vencdate then  to_char((fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision)), 'FM9G999G990D00') else to_char(0, 'FM9G999G990D00') end as multa, "\
                                    " case when paid <> true and current_date > vencdate then  to_char(((fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision))+fac.value)/100*(cast(replace((select juros from ticketconfig limit 1), ',','.') as double precision))/30, 'FM9G999G990D00') else to_char(0, 'FM9G999G990D00') end as jurosdia, "\
                                    " case when paid <> true and current_date > vencdate then "\
                                    " to_char((fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value)+ "\
                                    " (fac.value /100 * cast(replace((select multa  from ticketconfig limit 1), ',','.') as double precision) +fac.value) /100* "\
                                    " (cast(replace((select juros from ticketconfig limit 1), ',','.') as double precision)/30* (current_date - vencdate)), 'FM9G999G990D00')"\
                                    " else to_char(fac.value, 'FM9G999G990D00') end as value "\
                                    " ,case when fac.paid = true then 'PAGO' when current_date > fac.vencdate then 'VENCIDA' else 'A VENCER' end as status, "\
                                    " case when tkt.type is null then fat.description else case when tkt.type = 0 then 'CONDOMÍNIO' else 'TX EXTRA'  || '(' || reas.description || ')'  end end as accounttype, "\
                                    " case when fac.paid = true then 'P' else case when vencdate > current_date then 'T' else "\
                                    " case when vencdate < current_date then 'V' else 'H' end end end as situation, "\
                                    " case when fac.valuepaid is null then 0 else fac.valuepaid end as valuepaid, "\
                                    " case when t.name is null then '' else t.name end as torre,"\
                                    " case when a.id is null then 0 else a.id end as apart,"\
                                    " ext.valueadv as valueadv "\
                                    " from fin_accounttoreceive fac inner join fin_accounttype fat on fat.id = fac.accounttypeid "\
                                    " %1 join ticket tkt on tkt.accountid = fac.id %2 "\
                                    " %1 join dweller c on tkt.clientid = c.id "\
                                    " %1 join ap a on c.ap = a.id "\
                                    " %1 join tower t on c.tower = t.id "\
                                    " where fac.removed = false %3 %4"


    QString aux;
    QString Where = " and ";
    QString InnerJoinExtrTax = QString(" left join extratx ext on ext.id = tkt.extratxid left join reasonextratx reas on ext.motivo = reas.id");
    QString JoinType = " left ";
    QString OrderBy;
    QString WhereVencidas;
    QString DateLogic;

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

        DateLogic = QString(" %1 between '%2' and '%3' ")
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
       WhereVencidas += QString(" and c.id = %1 ").arg(m_pessoaAP->index(ui->comboBoxPessoaAp->currentIndex(),0).data().toInt());
    }

    if( ui->groupBoxApenasTipo->isChecked() )
    {
        JoinType  = " inner ";

        if( ui->radioButtonCondominio->isChecked() )
        {
            if( Where.length() > 5)
                 Where += " and ";
            Where += QString(" tkt.type = %1 ").arg(0);
            WhereVencidas += QString(" and tkt.type = %1 ").arg(0);
        }
        else
        {
            int currentid = ui->comboBoxTypeTxExtr->model()->data(ui->comboBoxTypeTxExtr->model()->index(ui->comboBoxTypeTxExtr->currentIndex(), 0)).toInt();
            if( currentid > 0 )
            {
                InnerJoinExtrTax = QString(" inner join extratx ext on ext.id = tkt.extratxid and ext.motivo = %1 inner join reasonextratx reas on ext.motivo = reas.id ").arg(currentid);
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
            WhereVencidas += " and t.id = 1 ";
        }
        else if( ui->radioButtonMarine->isChecked())
        {
            Where += " t.id = 2 ";
            WhereVencidas += " and t.id = 2 ";
        }
        else
        {
            Where += " t.id = 3 ";
            WhereVencidas += " and t.id = 3 ";
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
    else if (ui->checkBoxNotPaid->isChecked())
    {
        Where += QString(" fac.paid = false and fac.vencdate > '%1' ").arg(QDate::currentDate().toString(FMT_DATE_DB));
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

    //groupBoxFilterData
    if( ui->checkBoxVencidas->isChecked() )
    {
        if( !ui->groupBoxFilterData->isChecked() )
        {
            QString WhereUnion = QString(" and ( fac.paid = false and fac.vencdate < '%1') %2 ")
                    .arg(QDate::currentDate().toString(FMT_DATE_DB))
                    .arg(WhereVencidas);
            QString Union = "( " + QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(JoinType).arg(InnerJoinExtrTax).arg(WhereUnion).arg(OrderBy);
            aux  =  QString(" %1) union ( ")
                    .arg(Union);
            ////// bueno 15/12/2017
            if( !ui->checkBoxPaid->isChecked() && !ui->checkBoxNotPaid->isChecked() )
            {
                if( Where.length() )
                    Where += " and ";
                Where += QString(" fac.vencdate < '%1' ").arg(QDate::currentDate().toString(FMT_DATE_DB));
            }
            ////////


            aux += QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(JoinType).arg(InnerJoinExtrTax).arg(Where).arg(OrderBy);
            aux += " ) ";
            aux += OrderBy.replace("t.name", "torre").replace("c.name", "client").replace("fac.paid", "status").replace("a.id", "apart").remove("fac.");
        }
        else
        {
            QString WhereUnion = QString(" and ( fac.paid = false and %1 and fac.vencdate < '%2') %3 ")
                    .arg(DateLogic)
                    .arg(QDate::currentDate().toString(FMT_DATE_DB))
                    .arg(WhereVencidas);
            QString Union = "( " + QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(JoinType).arg(InnerJoinExtrTax).arg(WhereUnion).arg(OrderBy);
            aux  =  QString(" %1) union ( ")
                    .arg(Union);
            aux += QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(JoinType).arg(InnerJoinExtrTax).arg(Where).arg(OrderBy);
            aux += " ) ";
            aux += OrderBy.replace("t.name", "torre").replace("c.name", "client").replace("fac.paid", "status").replace("a.id", "apart").remove("fac.");
        }

    }
    else
    {

        aux = QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(JoinType).arg(InnerJoinExtrTax).arg(Where).arg(OrderBy);
    }
    return aux;
}

void actoReceiveReport::TestMountSQL()
{
    ui->dateEditFrom->setDate(QDate::fromString("01/07/2017","dd/MM/yyyy"));
    ui->dateEditUntil->setDate(QDate::fromString("15/07/2017","dd/MM/yyyy"));

    ui->checkBoxNotPaid->setChecked(false);
    ui->checkBoxPaid->setChecked(false);
    ui->groupBoxFilterData->setChecked(false);
    ui->checkBoxOrderApTorre->setChecked(false);

    QString SQLReport = MountSQLReport();
    debug_message("SQL Report:%s\n",SQLReport.toLatin1().data());
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
  int total = 0, totalPaid = 0, totaladv=0;

  QString SQLReport = MountSQLReport();

  select->setQuery(SQLReport);


  debug_message("SQL Report:%s\n",SQLReport.toLatin1().data());

  for (int index = 0; index < select->rowCount(); index++)
  {
      total       += QRadMoney::StrToInt(select->record(index).value("value").toString());
      totalPaid   += QRadMoney::StrToInt(select->record(index).value("valuepaid").toString());
      totaladv    += QRadMoney::StrToInt(select->record(index).value("valueadv").toString());
  }


  report->setQuery("account", SQLReport);

  report->setAttributeMoneyValue("TOTAL", total);
  report->setAttributeMoneyValue("TOTAL_PAID", totalPaid);
  report->setAttributeMoneyValue("TOTAL_CUSTAS", totaladv);

  if ( !report->show() )
  {
      QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
  }

  delete report;
  delete select;
  this->setEnabled(true);

}

void actoReceiveReport::EnableDisableVencidas()
{
   if( ui->radioButtonFilterPayment->isChecked() )
   {
       ui->checkBoxVencidas->setChecked(false);
       ui->checkBoxVencidas->setEnabled(false);
   }
   else
   {
       ui->checkBoxVencidas->setEnabled(true);
   }
}
