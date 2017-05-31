#include "financierplugin.h"
#include "managerticketconfig.h"
#include "managerbankaccount.h"
#include "managermetreage.h"

FinancierPlugin::FinancierPlugin()
{
    m_accountCalendar = 0;
    m_accountTypeReport = 0;
    m_accountHistoryReport = 0;
    m_accountToReceiveManager = 0;
    m_accountTypeManager = 0;
    m_bankManager = 0;
    m_accountToPayManager = 0;
}

FinancierPlugin::~FinancierPlugin()
{
    delete m_accountCalendar;
    delete m_accountTypeReport;
    delete m_accountHistoryReport;
    delete m_accountToReceiveManager;
    delete m_accountTypeManager;
    delete m_bankManager;
    delete m_accountToPayManager;

    m_container->destroy();
}

void FinancierPlugin::onLoad(QRadPluginContainer* container)
{
    m_container = container;

    appendAction(ACTION_SHOW_FINANCIER_ACCOUNTTYPE);
    appendAction(ACTION_SHOW_FINANCIER_BANK);
    appendAction(ACTION_SHOW_FINANCIER_ACCOUNTTOPAY);
    appendAction(ACTION_SHOW_FINANCIER_ACCOUNTTORECEIVE);
    appendAction(ACTION_SHOW_FINANCIER_ACCOUNTHISTORYREPORT);
    appendAction(ACTION_SHOW_FINANCIER_ACCOUNTTYPEREPORT);
    appendAction(ACTION_SHOW_FINANCIER_ACCOUNTCALENDAR);
    appendAction(ACTION_SHOW_ENTERBILLET);
    appendAction(ACTION_SHOW_FINANCIER_TICKETMANAGER);
    appendAction(ACTION_SHOW_FINANCIER_BANKACCOUNTMANAGER);
    appendAction(ACTION_SHOW_METREAGE);

}

void FinancierPlugin::Process( const QString& action )
{
#if 0
    if (!CanExecute())
        return;
#endif
#pragma warning "Add lic interface to test if can execut"


    if ( ACTION_SHOW_FINANCIER_ACCOUNTTYPE == action )
    {
        if (!m_accountTypeManager)
        {
            m_accountTypeManager = new AccountTypeManager(m_parent);
        }
        QRadConfig::centralizarWidget(m_accountTypeManager);
        m_accountTypeManager->show();
    }
    if ( ACTION_SHOW_FINANCIER_BANK == action )
    {
        if (!m_bankManager)
        {
            m_bankManager = new BankManager(m_parent);
        }
        QRadConfig::centralizarWidget(m_bankManager);
        m_bankManager->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTTOPAY == action )
    {
        if (!m_accountToPayManager)
        {
            m_accountToPayManager = new AccountToPayManager(m_parent);
        }
        QRadConfig::fullScreen(m_accountToPayManager);
        m_accountToPayManager->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTTORECEIVE == action )
    {
        if (!m_accountToReceiveManager)
        {
            m_accountToReceiveManager = new AccountToReceiveManager(m_parent);
        }
        QRadConfig::fullScreen(m_accountToReceiveManager);
        m_accountToReceiveManager->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTHISTORYREPORT == action )
    {
        if (!m_accountHistoryReport)
        {
            m_accountHistoryReport = new AccountHistoryReport(m_parent);
        }
        QRadConfig::centralizarWidget(m_accountHistoryReport);
        m_accountHistoryReport->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTTYPEREPORT == action )
    {
        if (!m_accountTypeReport)
        {
            m_accountTypeReport = new AccountTypeReport(m_parent);
        }
        QRadConfig::centralizarWidget(m_accountTypeReport);
        m_accountTypeReport->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTCALENDAR == action )
    {
        if (!m_accountCalendar)
        {
            m_accountCalendar = new AccountCalendar(m_parent);
        }
        QRadConfig::centralizarWidget(m_accountCalendar);
        m_accountCalendar->show();
    }
    else if( ACTION_SHOW_FINANCIER_TICKETMANAGER == action )
    {
        Managerticketconfig *tick = new Managerticketconfig ;
        QRadConfig::centralizarWidget(tick);
        tick->exec();
        delete tick;
    }
    else if( ACTION_SHOW_FINANCIER_BANKACCOUNTMANAGER == action )
    {
        Managerbankaccount *baccount = new Managerbankaccount ;
        QRadConfig::centralizarWidget(baccount);
        baccount->exec();
        delete baccount;
    }
    else if ( ACTION_SHOW_METREAGE == action )
    {
        Managermetreage *pMetreage = new Managermetreage;
        QRadConfig::centralizarWidget(pMetreage);
        pMetreage->exec();
        delete pMetreage;
    }
}

void FinancierPlugin::setParam(QString paramName, QVariant paramValue)
{
    if (paramName == "EntranceId")
    {
        m_entranceId = paramValue.toInt();
    }
}

QVariant FinancierPlugin::getParam(QString)
{
    return NULL;
}

// Q_EXPORT_PLUGIN2(financierplugin, FinancierPlugin)
