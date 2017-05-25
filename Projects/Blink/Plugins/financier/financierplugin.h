#ifndef FINANCIERPLUGIN_H
#define FINANCIERPLUGIN_H

#include <QtCore>

#include "qradplugininterface.h"
#include "qradplugincontainer.h"
#include "accounttypemanager.h"
#include "bankmanager.h"
#include "accounttopaymanager.h"
#include "accounttoreceivemanager.h"
#include "accounthistoryreport.h"
#include "accounttypereport.h"
#include "accountcalendar.h"
#include "qradshared.h"
#include "qradconfig.h"

class FinancierPlugin : public QRadPluginInterface 
{
    Q_OBJECT;
    Q_INTERFACES( QRadPluginInterface );
    Q_PLUGIN_METADATA(IID "financierplugin" FILE "financier.json")

    public:
        FinancierPlugin();
        ~FinancierPlugin();

        void onLoad(QRadPluginContainer*);
    private:
        QRadPluginContainer      *m_container;
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
