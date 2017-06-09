#ifndef _BUILDTKT_H
#define _BUILDTKT_H

#include <QObject>
#include  "ticketconfig.h"
#include  "bankaccount.h"
#include  "bankmodel.h"
#include  "maincompanyV2.h"
#include  "dweller.h"
#include  <QFileSystemWatcher>
#include  <QList>

#define TKT_DEFAULT_PDF_PATH "c:\\TEMP"

class Ticket
{
public:
    Ticket(Dweller *dweller, QString value, QDate date, QString NossoNumero, QString SeuNumero);
    virtual ~Ticket(){}
    Dweller *getDweller();
    QString getValue();
    QDate   getDate();
    QString getNossoNumero();
    QString getSeuNumero();
private:
    Dweller *m_dweller;
    QString m_value;
    QDate   m_date;
    QString m_NossoNumero;
    QString m_SeuNumero;

};



class BuildTkt : QObject
{
public:
      BuildTkt();
      ~BuildTkt();
      
       bool Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount);
       bool Print(bool bPrinter=false, QString strPath=TKT_DEFAULT_PDF_PATH );
       bool BuildShipping(QString strDir, QString FileName );
       bool AppendTicket(Dweller *pDweller, QString strValue, QDate dtVencto,
                         QString NossoNumero="",
                         QString SeuNumero="");
       bool AddTickets();

public slots:
       void DirModified(QString dir );

private:
      QList<Ticket *> m_tickets;
      int m_ShippNumber;
      int m_TktCount;
      bool m_bDirModified ;
      bool Send(QString cmd);
      void qSleep(int ms);
      MainCompany *m_pCompany;
      ticketconfig *m_pTktConfig;
      BankModel *m_pBank;
      bankaccount *m_pAccount;
      QFileSystemWatcher m_watcher;
      QString m_lastError;
};


#endif
