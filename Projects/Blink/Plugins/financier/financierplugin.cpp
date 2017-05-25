#include "financierplugin.h"

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

void FinancierPlugin::onLoad(DSMPluginContainer* container)
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
}

void FinancierPlugin::Process( const QString& action )
{
    if (!CanExecute())
        return;

    if ( ACTION_SHOW_FINANCIER_ACCOUNTTYPE == action )
    {
        if (!m_accountTypeManager)
        {
            m_accountTypeManager = new AccountTypeManager(m_parent);
        }
        dsmConfig::centralizarWidget(m_accountTypeManager);
        m_accountTypeManager->show();
    }
    if ( ACTION_SHOW_FINANCIER_BANK == action )
    {
        if (!m_bankManager)
        {
            m_bankManager = new BankManager(m_parent);
        }
        dsmConfig::centralizarWidget(m_bankManager);
        m_bankManager->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTTOPAY == action )
    {
        if (!m_accountToPayManager)
        {
            m_accountToPayManager = new AccountToPayManager(m_parent);
        }
        dsmConfig::fullScreen(m_accountToPayManager);
        m_accountToPayManager->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTTORECEIVE == action )
    {
        if (!m_accountToReceiveManager)
        {
            m_accountToReceiveManager = new AccountToReceiveManager(m_parent);
        }
        dsmConfig::fullScreen(m_accountToReceiveManager);
        m_accountToReceiveManager->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTHISTORYREPORT == action )
    {
        if (!m_accountHistoryReport)
        {
            m_accountHistoryReport = new AccountHistoryReport(m_parent);
        }
        dsmConfig::centralizarWidget(m_accountHistoryReport);
        m_accountHistoryReport->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTTYPEREPORT == action )
    {
        if (!m_accountTypeReport)
        {
            m_accountTypeReport = new AccountTypeReport(m_parent);
        }
        dsmConfig::centralizarWidget(m_accountTypeReport);
        m_accountTypeReport->show();
    }
    else if ( ACTION_SHOW_FINANCIER_ACCOUNTCALENDAR == action )
    {
        if (!m_accountCalendar)
        {
            m_accountCalendar = new AccountCalendar(m_parent);
        }
        dsmConfig::centralizarWidget(m_accountCalendar);
        m_accountCalendar->show();
    }
    else if ( ACTION_SHOW_ENTERBILLET == action )
    {
        EnterBillet *enterBillet = new EnterBillet(m_parent);
        enterBillet->GetEntranceId(m_entranceId);
        dsmConfig::centralizarWidget(enterBillet);
        enterBillet->exec();
        delete enterBillet;
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
