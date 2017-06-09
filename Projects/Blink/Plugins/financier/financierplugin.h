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

#define ACTION_SHOW_FINANCIER_ACCOUNTTYPE           "ShowAccountType"
#define ACTION_SHOW_FINANCIER_BANK                  "ShowBank"
#define ACTION_SHOW_FINANCIER_ACCOUNTTOPAY          "ShowAccountToPay"
#define ACTION_SHOW_FINANCIER_ACCOUNTTORECEIVE      "ShowAccountToReceive"
#define ACTION_SHOW_FINANCIER_ACCOUNTHISTORYREPORT  "ShowAccountHistoryReport"
#define ACTION_SHOW_FINANCIER_ACCOUNTTYPEREPORT     "ShowAccountTypeReport"
#define ACTION_SHOW_FINANCIER_ACCOUNTCALENDAR       "ShowAccountCalendar"
#define ACTION_SHOW_ENTERBILLET                     "ShowEnterBillet"
#define ACTION_SHOW_FINANCIER_TICKETMANAGER         "ManageTicketConfig"
#define ACTION_SHOW_FINANCIER_BANKACCOUNTMANAGER    "ManageBankAccount"
#define ACTION_SHOW_METREAGE                        "ManageMetreage"
#define ACTION_MANAGE_BANK_ACCOUNT                  "ManageBankAccount"
#define ACTION_MANAGE_TICKET                        "ManageTicket"
#define ACTION_MANAGE_SUPPLIER                      "ManageSupplier"
#define ACTION_MANAGE_SHIPPER                      "ManageShipper"


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
