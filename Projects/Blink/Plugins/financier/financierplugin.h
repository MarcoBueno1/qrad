#ifndef FINANCIERPLUGIN_H
#define FINANCIERPLUGIN_H

#include <QtCore>

#include "dsmplugininterface.h"
#include "dsmpluginreport.h"
#include "dsmplugincontainer.h"
#include "accounttypemanager.h"
#include "bankmanager.h"
#include "accounttopaymanager.h"
#include "accounttoreceivemanager.h"
#include "accounthistoryreport.h"
#include "accounttypereport.h"
#include "accountcalendar.h"
#include "enterbillet.h"
#include "dsmshared.h"
#include "dsmconfig.h"

class FinancierPlugin : public DSMPluginInterface, public DSMPluginReport
{
    Q_OBJECT;
    Q_INTERFACES( DSMPluginInterface );
    Q_PLUGIN_METADATA(IID "financierplugin" FILE "financier.json")

    public:
        FinancierPlugin();
        ~FinancierPlugin();

        void onLoad(DSMPluginContainer*);
    private:
        DSMPluginContainer      *m_container;
        AccountTypeManager      *m_accountTypeManager;
        BankManager             *m_bankManager;
        AccountToPayManager     *m_accountToPayManager;
        AccountToReceiveManager *m_accountToReceiveManager;
        AccountHistoryReport    *m_accountHistoryReport;
        AccountTypeReport       *m_accountTypeReport;
        AccountCalendar         *m_accountCalendar;

        int     m_entranceId;

    private slots:
        void Process( const QString& action );
        void setParam(QString, QVariant);
        QVariant getParam(QString);
};

#endif // FINANCIERPLUGIN_H
