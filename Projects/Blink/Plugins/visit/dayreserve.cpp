#include "dayreserve.h"
#include "ui_dayreserve.h"
#include "qradshared.h"
#include <QMessageBox>
#include "reserve.h"
#include "newdayreserve.h"

#define SQL_RES " select time_start as \"Inicio\", time_end as \"Fim\", "\
                " a.numero || ' ' || substring(t.name from 1 for 1) || '-' || d.name as \"Responsável\", r.id "\
                " from reserve r "\
                " inner join dweller d on d.id = r.responsable "\
                " inner join ap a on a.id = d.ap "\
                " inner join tower t on t.id = d.tower "\
                " where date_start = '%1' and commona_id = %2 and canceled = false "

DayReserve::DayReserve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DayReserve)
{
    ui->setupUi(this);
    m_model = new QSqlQueryModel;
    m_CenterDelegate = new ColumnCenter;
    m_LeftDelegate = new ColumnLeft ;

    connect(ui->pushButtonNew, SIGNAL(clicked(bool)),this,SLOT(NewClicked(bool)));
    connect(ui->pushButtonCancelarReserva, SIGNAL(clicked(bool)),this,SLOT(CenceledClicked(bool)));
    connect(ui->pushButtonEdit, SIGNAL(clicked(bool)),this,SLOT(EditClicked(bool)));
}

DayReserve::~DayReserve()
{
    delete m_model;
    delete m_CenterDelegate;
    delete m_LeftDelegate;
    delete ui;
}

void DayReserve::NewClicked(bool)
{
 // chamar new

    NewDayReserve *newD = new NewDayReserve;
    if( QDialog::Accepted == newD->Exec(m_CommonId,m_CommonName, m_day))
            RunSQL();
    delete newD;
}

void DayReserve::EditClicked(bool)
{
    int id = m_model->index(ui->tableView->currentIndex().row(),3).data().toInt();
    if(1>id)
    {
        QMessageBox::warning(this, "Oops!", "Selecione uma reserva para editar!");
        ui->tableView->selectRow(0);
        return;
    }

    NewDayReserve *newD = new NewDayReserve;
    if( QDialog::Accepted == newD->Edit(id,m_CommonName))
            RunSQL();
    delete newD;

}

void DayReserve::RunSQL()
{
    QString SQL = QString(SQL_RES).arg(m_day.toString(FMT_DATE_DB)).arg(m_CommonId);

    m_model->setQuery(SQL);

    ui->tableView->setModel(m_model);
    ui->tableView->hideColumn(3);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegateForColumn(0, m_CenterDelegate);
    ui->tableView->setItemDelegateForColumn(1, m_CenterDelegate);
    ui->tableView->setItemDelegateForColumn(2, m_LeftDelegate);

    ui->tableView->selectRow(0);
    QString text = QString( "Dia: %1, Reservas: %2").arg(m_day.toString(FMT_DATE)).arg(m_model->rowCount());
    ui->groupBox->setTitle(text);
}

int DayReserve::Exec(int CommonId, QString CommonName, QDate day)
{
    m_CommonId   = CommonId;
    m_CommonName = CommonName;
    m_day = day;

    setWindowTitle( QString("Reserva de %1, Dia: %2").arg(CommonName).arg(day.toString(FMT_DATE)));


    RunSQL();


    return exec();
}
void DayReserve::CenceledClicked(bool)
{
    QString Name;
    QTime StartTime;
    QTime EndTime;

    StartTime = m_model->index(ui->tableView->currentIndex().row(),0).data().toTime();
    EndTime = m_model->index(ui->tableView->currentIndex().row(),1).data().toTime();
    Name = m_model->index(ui->tableView->currentIndex().row(),2).data().toString();

    QString strText = QString( "Tem certeza de que deseja cancelar a reserva:\n Inicio:%1\n Fim:%2\n Responsável:%3" )
            .arg(StartTime.toString(FMT_TIME))
            .arg(EndTime.toString(FMT_TIME))
            .arg(Name);

   if( QMessageBox::No == QMessageBox::question(this,
                         QString("Atenção"),
                         strText,
                         QMessageBox::Yes | QMessageBox::No, QMessageBox::No))
   {
       ui->tableView->selectRow(0);

       return;
   }

    reserve *res  = reserve::findByid(m_model->index(ui->tableView->currentIndex().row(),3).data().toInt(),
                                      true);
    if( !res )
    {
        QMessageBox::warning(this,
                             QString("Oops!"),
                             "Impossível cancelar, não encontrado registro no banco de dados");
        ui->tableView->selectRow(0);

        return;
    }
    res->updatecanceled(true);
    delete res;
    RunSQL();
}
