#include "financierplugin.h"
#include "managerticketconfig.h"
#include "managerbankaccount.h"
#include "managermetreage.h"
#include "managerticket.h"
#include "managersupplier.h"
#include "managershipper.h"
#include "showbankreturn.h"
#include "ticketcontroller.h"
#include "managerfiles.h"
#include "accounttopayregister.h"

FinancierPlugin::FinancierPlugin()
{
    m_accountCalendar = 0;
    m_accountTypeReport = 0;
    m_accountHistoryReport = 0;
    m_accountToReceiveManager = 0;
    m_accountTypeManager = 0;
    m_bankManager = 0;
    m_accountToPayManager = 0;
    m_LastId =0;
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

//    Q_INIT_RESOURCE("resources.qrc");

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
    appendAction(ACTION_MANAGE_TICKET);
    appendAction(ACTION_MANAGE_SUPPLIER);
    appendAction(ACTION_MANAGE_SHIPPER);
    appendAction(ACTION_READ_SHIPP);
    appendAction(ACTION_MANAGE_FILES);
    appendAction(ACTION_NEW_ACCOUNTTOPAY);
    appendAction(TEST_EXPORT_PDF);
//    appendAction(ACTION_TEST_SUPPLIER_HTTP_RESP);


}

void FinancierPlugin::Process( const QString& action )
{
#if 0
    if (!CanExecute())
        return;
#endif
#pragma warning "Add lic interface to test if can execut"

    m_LastId = 0;

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
        Managerticketconfig *tick = new Managerticketconfig(m_parent) ;
        QRadConfig::centralizarWidget(tick);
        tick->exec();
        delete tick;
    }
    else if( ACTION_SHOW_FINANCIER_BANKACCOUNTMANAGER == action )
    {
        Managerbankaccount *baccount = new Managerbankaccount(m_parent) ;
        QRadConfig::centralizarWidget(baccount);
        baccount->exec();
        delete baccount;
    }
    else if ( ACTION_SHOW_METREAGE == action )
    {
        Managermetreage *pMetreage = new Managermetreage(m_parent);
        QRadConfig::centralizarWidget(pMetreage);
        pMetreage->exec();
        delete pMetreage;
    }
    else if ( ACTION_MANAGE_TICKET == action )
    {
        Managerticket *pTicket = new Managerticket(m_parent);
        QRadConfig::centralizarWidget(pTicket);
        pTicket->exec();
        delete pTicket;
    }
    else if ( ACTION_MANAGE_SUPPLIER == action )
    {
        Managersupplier *pSupplier = new Managersupplier(m_parent);
        QRadConfig::centralizarWidget(pSupplier);
        pSupplier->exec();
        delete pSupplier;
    }
    else if ( ACTION_MANAGE_SHIPPER == action )
    {
        Managershipper *pShipper = new Managershipper(m_parent);
        QRadConfig::centralizarWidget(pShipper);
        pShipper->exec();
        delete pShipper;
    }
    else if( ACTION_READ_SHIPP == action)
    {
        QStringList paths = QRadConfig::GetAndPersistDir("RetFile", "","Selecionar Arquivo de Retorno","Arquivos Retorno (*.ret *.RET)",m_parent);

        QList<BankTicket*> list;
        ShowBankReturn *ParsePay = new ShowBankReturn(m_parent) ;

        #ifdef _WIN32
//            path = path.remove("file://");
        #else
//            path = "C:\\Dvl\\CN12067A.RET";
        #endif

            if(ParsePay->Exec(&list, paths))
            {
                TicketController::UpdateTickets(&list);
            }
            delete ParsePay;
    }
    else if( ACTION_MANAGE_FILES == action)
    {
        Managerfiles *pFiles = new Managerfiles(m_parent);
        QRadConfig::centralizarWidget(pFiles);
        pFiles->exec();
        delete pFiles;
    }
    else if( ACTION_NEW_ACCOUNTTOPAY == action )
    {
        AccountToPayRegister *RegAccount = new AccountToPayRegister(m_parent);
        if( QDialog::Accepted == RegAccount->exec())
        {
            m_LastId = RegAccount->GetLastInsertedId();
        }
        delete RegAccount;

    }
    else if( TEST_EXPORT_PDF ==  action )
    {
        Managerticket *ptkts = new Managerticket(m_parent);
//        QRadConfig::centralizarWidget(pFiles);
        ptkts->TestExportPDF();
        delete ptkts;

    }
//    else if( ACTION_TEST_SUPPLIER_HTTP_RESP == action)
//    {
//        Editsupplier *edt = new Editsupplier(m_parent) ;
//        edt->replyFinished(NULL) ;
//        delete edt;
//    }
}

void FinancierPlugin::setParam(QString paramName, QVariant paramValue)
{
    if (paramName == "EntranceId")
    {
        m_entranceId = paramValue.toInt();
    }
}

QVariant FinancierPlugin::getParam(QString paramname)
{
    if( paramname == "lastinsertedid")
        return m_LastId;
    return NULL;
}

// Q_EXPORT_PLUGIN2(financierplugin, FinancierPlugin)
