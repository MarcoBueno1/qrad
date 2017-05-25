#include "accountcalendar.h"

#define SQL_SELECT_ACCOUNTTOPAY     "select fac.id, fac.description, fac.vencdate  from dsm_fin_accounttopay fac where fac.removed = false and to_char(fac.vencdate, 'MM/yyyy') = '%1' and fac.paid = false order by fac.vencdate, fac.description"
#define SQL_SELECT_ACCOUNTTORECEIVE "select fac.id, fac.description, fac.vencdate  from dsm_fin_accounttoreceive fac where fac.removed = false and to_char(fac.vencdate, 'MM/yyyy') = '%1' and fac.paid = false order by fac.vencdate, fac.description"

AccountCalendar::AccountCalendar(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::AccountCalendar)
{
    m_ui->setupUi(this);

    m_accountModel = new QSqlQueryModel;

    connect(m_ui->calendarAccount, SIGNAL(currentPageChanged(int,int)), this, SLOT(UpdateCalendar(int,int)));
    connect(m_ui->calendarAccount, SIGNAL(selectionChanged()), this, SLOT(UpdateDate()));
    connect(m_ui->calendarAccount, SIGNAL(activated(QDate)), this, SLOT(ShowDetail(QDate)));
}

AccountCalendar::~AccountCalendar()
{
    delete m_accountModel;
    delete m_ui;
}

void AccountCalendar::changeEvent(QEvent *e)
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

void AccountCalendar::showEvent(QShowEvent *e)
{
    Q_UNUSED(e);
    UpdateCalendar(m_ui->calendarAccount->yearShown(), m_ui->calendarAccount->monthShown());
}

void AccountCalendar::closeEvent(QCloseEvent *event)
{
    if (QMessageBox::question(this, MSG_INFO_TITLE, MSG_QUESTION_CLOSE, QMessageBox::Yes|QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    {
        event->ignore();
    }
}

void AccountCalendar::keyPressEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Escape:
        {
            this->close();
            break;
        }
        case Qt::Key_F1:
        {
            ShowDetail(m_date);
            break;
        }

        default:
            QWidget::keyPressEvent( event );
            break;
    }
}

void AccountCalendar::UpdateDate(void)
{
    m_date = m_ui->calendarAccount->selectedDate();
}

void AccountCalendar::ShowDetail(QDate date)
{
    bool flag = false;

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
}

void AccountCalendar::UpdateCalendar(int year,int month)
{
    QDate   date(year, month, 1);
    bool    flag;

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
}

