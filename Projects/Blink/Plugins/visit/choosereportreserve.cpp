#include "choosereportreserve.h"
#include "ui_choosereportreserve.h"
#include <QMessageBox>
#include <QCompleter>
#include "qradshared.h"
#include "qraddebug.h"
#include "qradreportmanager.h"

#define SQL_COMBO_LOCAL "select name, id from common_area where removed<> true and tp <> 2 and name <> ''  order by name"
#define SQL_COMBO_PESSOA " select a.numero || ' ' || substring(t.name from 1 for 1) || '-' || d.name as \"Responsável\","\
                         " d.id "\
                         " from dweller d  "\
                         " inner join ap a on a.id = d.ap  "\
                         " inner join tower t on t.id = d.tower  "\
                         " where d.removed <> true and d.movedout <> true "\
                         " and d.id in ( select responsable from reserve where date_start between '%1' and '%2' and canceled <> true) "\
                         " order by t.id, a.id"

#define SQL_REPORT " select ca.name, to_char(date_start, 'dd-mm-yyyy') as date_start , time_start , time_end , "\
                   " a.numero || ' ' || substring(t.name from 1 for 1) || '-' || d.name as \"responsavel\" "\
                   " from reserve r inner join dweller d on d.id = r.responsable "\
                   " inner join ap a on a.id = d.ap "\
                   " inner join tower t on t.id = d.tower "\
                   " inner join common_area ca on ca.id = r.commona_id "\
                   " where date_start between '%1' and '%2' %3 %4 canceled <> true "\
                   " order by date_start, time_start"


//" where date_start between '%1' and '%2' and commona_id = %3 %4 and canceled <> true "


ChooseReportReserve::ChooseReportReserve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChooseReportReserve)
{
    ui->setupUi(this);

    m_modelLocal =  new QSqlQueryModel;
    m_modelLocal->setQuery(SQL_COMBO_LOCAL);
    ui->comboBoxLocal->setModel(m_modelLocal);
    ui->comboBoxLocal->setModelColumn(0);
    if( ui->comboBoxLocal->completer() )
        ui->comboBoxLocal->completer()->setFilterMode(Qt::MatchContains );

    FillComboPerson();

    ui->dateEditFrom->setDate(QDate::currentDate());
    ui->dateEditUntil->setDate(QDate::currentDate());

    connect(ui->pushButton,SIGNAL(clicked(bool)), this,SLOT(BuildReport(bool)));
    connect(ui->dateEditUntil,SIGNAL(dateChanged(QDate)), this,SLOT(dateChanged(QDate)));
    connect(ui->dateEditFrom,SIGNAL(dateChanged(QDate)), this,SLOT(dateChanged(QDate)));

    setWindowTitle("Relatório de Reservas");

}

ChooseReportReserve::~ChooseReportReserve()
{
    delete m_modelLocal ;
    delete m_modelPessoa;
    delete ui;
}
void ChooseReportReserve::BuildReport(bool)
{
 /// consistencias
 ///
 ///

    if(ui->dateEditUntil->date() < ui->dateEditFrom->date())
    {
        QMessageBox::warning(this,
                             "Oops!",
                             "A data de início não pode ser maior que a data final");
        ui->dateEditUntil->setDate(ui->dateEditFrom->date());
        return;
    }


    QString strLocal;
    if( ui->groupBoxLocal->isChecked() )
    {
        int localId = m_modelLocal->index(ui->comboBoxLocal->currentIndex(),1).data().toInt();
        if ( 1>localId )
        {
            QMessageBox::warning(this,
                                 "Oops!",
                                 "Selecione o Local!");

            ui->comboBoxLocal->setFocus();
            return;
        }
        strLocal = QString(" and commona_id = %1 ").arg(localId);
    }

    QString pessoa = " and ";
    if( ui->groupBoxOnly->isChecked())
    {
        int idPessoa = m_modelPessoa->index(ui->comboBoxPessoa->currentIndex(),1).data().toInt();
        if( 1>idPessoa)
        {
            QMessageBox::warning(this,
                                 "Oops!",
                                 "Selecione o Morador!");
            ui->comboBoxPessoa->setFocus();
            return;

        }
        pessoa = QString(" and responsable = %1 and ").arg(idPessoa);
    }

    QString SQL = QString(SQL_REPORT)
            .arg(ui->dateEditFrom->date().toString(FMT_DATE_DB))
            .arg(ui->dateEditUntil->date().toString(FMT_DATE_DB))
            .arg(strLocal)
            .arg(pessoa);

    debug_message("SQL:%s\n", SQL.toLatin1().data());

    this->setEnabled(false);
    QRadReportManager   *report = new QRadReportManager();
    if ( !report->load( "reserve" ) )
    {
       QMessageBox::critical( this, "Erro", "Falha ao carregar arquivo de relatório(reserve)" );
       delete report;
       this->setEnabled(true);

       return;
    }
    report->setQuery("account", SQL);

    QString DEATE = ui->dateEditFrom->date() == ui->dateEditUntil->date()?QString("Data: %1").arg(ui->dateEditFrom->date().toString(FMT_DATE)):
                  QString("DE %1 ATÉ: %2").arg(ui->dateEditFrom->date().toString(FMT_DATE)).arg(ui->dateEditUntil->date().toString(FMT_DATE));

    report->setAttributeValue(QString("NOME"), QString("RESERVA DE %1").arg(ui->groupBoxLocal->isChecked()?
                                                                            ui->comboBoxLocal->currentText().toUpper():
                                                                            "ÁREAS COMUNS"));
    report->setAttributeValue(QString("DEATE"), DEATE );
    if ( !report->show() )
    {
        QMessageBox::critical( this, windowTitle(), QString::fromUtf8( "Não foi possível exibir o relatório." ) );
    }

    delete report;

    this->setEnabled(true);
}
void ChooseReportReserve::FillComboPerson()
{
    m_modelPessoa =  new QSqlQueryModel;

    QString query =  QString(SQL_COMBO_PESSOA)
            .arg(QDate::currentDate().toString(FMT_DATE_DB))
            .arg(QDate::currentDate().toString(FMT_DATE_DB));

    m_modelPessoa->setQuery(query);
    ui->comboBoxPessoa->setModel(m_modelPessoa);
    ui->comboBoxPessoa->setModelColumn(0);
    if( ui->comboBoxPessoa->completer() )
        ui->comboBoxPessoa->completer()->setFilterMode(Qt::MatchContains );

    ui->comboBoxPessoa->setToolTip("Moradores que tem reserva no intervalo de dadas selecionado");
}

void ChooseReportReserve::dateChanged(QDate)
{
    FillComboPerson();
}
