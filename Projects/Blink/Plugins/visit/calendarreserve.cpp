#include "calendarreserve.h"
#include "ui_calendarreserve.h"
#include <QMessageBox>
#include "qraddebug.h"
#include "dayreserve.h"

#define SQL_RES "select * from reserve where date_start between '%1' and '%2'"

calendarreserve::calendarreserve(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::calendarreserve)
{
    ui->setupUi(this);

    m_model = new QSqlQueryModel;

    connect(ui->calendarWidget, SIGNAL(currentPageChanged(int,int)), this, SLOT(UpdateCalendar(int,int)));
    connect(ui->calendarWidget, SIGNAL(selectionChanged()), this, SLOT(UpdateDate()));
    connect(ui->calendarWidget, SIGNAL(activated(QDate)), this, SLOT(ShowDetail(QDate)));

    UpdateCalendar(QDate::currentDate().year(),QDate::currentDate().month());
}

calendarreserve::~calendarreserve()
{
    delete m_model;
    delete ui;
}
void calendarreserve::UpdateCalendar(int year,int month)
{
    QDate   date(year, month, 1);
    QDate   LastDay(year, month, date.daysInMonth());

    QList<QDate>dates;

    QString sql = QString(SQL_RES).arg(date.toString(FMT_DATE_DB)).arg(LastDay.toString(FMT_DATE_DB));

    m_model->setQuery(sql);

    debug_message("Antes do ClearFormat\n");
    ui->calendarWidget->ClearFormat(year, month);
    debug_message("Depois do ClearFormat\n");
    for(int i= 0; i <  m_model->rowCount(); i++)
    {
        debug_message("Loop: %d\n",i);
        dates.append(m_model->index(i,2).data().toDate());
        debug_message("End Loop: %d, data encontrada:%s\n",i,m_model->index(i,2).data().toDate().toString(FMT_DATE).toLatin1().data());
    }

    if( dates.count() )
    {
        debug_message("Chamando setReserveDate\n");
        ui->calendarWidget->setReserveDate(dates);
        debug_message("depois setReserveDate\n");
    }

    dates.clear();

//    setReserveDate(date.addDays(10));
/*    bool    flag;

    m_accountCalendar.clear();
    m_ui->calendarAccount->ClearFormat(year, month);

    m_accountModel->setQuery(QString(SQL_SELECT_ACCOUNTTOPAY).arg(date.toString(FMT_DATE_CAL)));

    for (int index = 0; index < m_accountModel->rowCount(); index++)
    {
        flag = true;
        for (int i = 0; i < m_accountCalendar.count(); i++)
        {
            if (m_accountModel->record(index).value("vencdate").toDate() == m_accountCalendar[i].date)
            {
                m_accountCalendar[i].descriptionToPay.push_back(m_accountModel->record(index).value("description").toString());
                flag = false;
            }
        }
        if (flag)
        {
            DSM_AccountCalendar_t accountCalendarItem;
            accountCalendarItem.date = m_accountModel->record(index).value("vencdate").toDate();
            accountCalendarItem.descriptionToPay.push_back(m_accountModel->record(index).value("description").toString());

            m_accountCalendar.push_back(accountCalendarItem);
        }
    }

    m_accountModel->setQuery(QString(SQL_SELECT_ACCOUNTTORECEIVE).arg(date.toString(FMT_DATE_CAL)));

    for (int index = 0; index < m_accountModel->rowCount(); index++)
    {
        flag = true;
        for (int i = 0; i < m_accountCalendar.count(); i++)
        {
            if (m_accountModel->record(index).value("vencdate").toDate() == m_accountCalendar[i].date)
            {
                m_accountCalendar[i].descriptionToReceive.push_back(m_accountModel->record(index).value("description").toString());
                flag = false;
            }
        }
        if (flag)
        {
            DSM_AccountCalendar_t accountCalendarItem;
            accountCalendarItem.date = m_accountModel->record(index).value("vencdate").toDate();
            accountCalendarItem.descriptionToReceive.push_back(m_accountModel->record(index).value("description").toString());

            m_accountCalendar.push_back(accountCalendarItem);
        }
    }

    foreach(DSM_AccountCalendar_t accountCalendarItem, m_accountCalendar)
    {
        m_ui->calendarAccount->setAccountDate(accountCalendarItem);
    }
*/
}

void calendarreserve::UpdateDate(void)
{
//    m_date = ui->calendarWidget->selectedDate();
}

void calendarreserve::ShowDetail(QDate date)
{
    DayReserve *dres = new DayReserve;

    dres->Exec(m_CommonId, m_CommonName, date);
    delete dres;


    UpdateCalendar(date.year(),date.month());

  //  QMessageBox::information(this, "Activacted", "Item Activated");
//    select


/*    bool flag = false;

    foreach(DSM_AccountCalendar_t accountCalendarItem, m_accountCalendar)
    {
        if (accountCalendarItem.date == date)
        {
            flag = true;
            break;
        }
    }

    if (flag)
    {
        AccountCalendarAux *accountCalendarAux = new AccountCalendarAux(this);
        accountCalendarAux->SendDate(date);
        QRadConfig::centralizarWidget(accountCalendarAux);
        accountCalendarAux->exec();
        delete accountCalendarAux;
    }
*/
}
int calendarreserve::Exec(int CommonId, QString CommonName )
{
  m_CommonId = CommonId;
  m_CommonName = CommonName;
  setWindowTitle( QString("Reserva de %1").arg(CommonName));
  return exec();
}
