#ifndef _BUILDTKT_H
#define _BUILDTKT_H

#include <QObject>
#include  "ticketconfig.h"
#include  "bankaccount.h"
#include  "bankmodel.h"
#include  "maincompanyV2.h"
#include  "dweller.h"
#include "bankticket.h"
#include  <QFileSystemWatcher>
#include  <QList>
#include "bankticketinterface.h"

#define TKT_DEFAULT_PDF_PATH "c:\\TEMP"

class Ticket
{
public:
    Ticket(Dweller *dweller,
           QString value,
           QDate date,
           QString NossoNumero,
           QString SeuNumero,
           QString Mensagem,
           QString Discount,
           QDate BuildDate);

    virtual ~Ticket(){}
    Dweller *getDweller();
    QString getValue();
    QDate   getDate();
    QString getNossoNumero();
    QString getSeuNumero();
    QString getMensagem();
    QString getDiscount();
    QDate   getBuildDate();
private:
    Dweller *m_dweller;
    QString m_value;
    QDate   m_date;
    QDate   m_BuildDate;
    QString m_NossoNumero;
    QString m_SeuNumero;
    QString m_Mensagem;
    QString m_Discount;
};



class BuildTkt : public QObject , public IBankTicketParser
{
public:
      BuildTkt(QString Path="");
      ~BuildTkt();
      
       bool Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount);
       bool Print(bool bPrinter, QString strPath);
       bool BuildShipping(QString strDir, QString FileName );
       bool AppendTicket(Dweller *pDweller,
                         QString strValue,
                         QDate dtVencto,
                         QString NossoNumero="",
                         QString SeuNumero="",
                         QString Mensagem="",
                         QString Discount="",
                         QDate BuildDate=QDate::currentDate());
       bool AddTickets();
       bool ExtractReturn(QList<BankTicket *> *tickets, QString strDir, QString FileName );

       bool Parse(QList<BankTicket*> *tikets,QString Path);
       bool Parse(QList<BankTicket*> *tikets);
       bool Print(bool bPrinter);
       bool BuildShipping(QString strDir);

public slots:
       void DirModified(QString dir );

private:
      void CleanUpList();
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
      QString m_SendPath;
      QString m_ReceivePath;
      QString m_SendFile;
      QString m_ReceiveFile;
      QString m_ReceiveFullPath;
      QString m_parsepath;
      int m_dwTimeout;
      QString m_lastRemDir;
      QString m_lastRemName;
      QString MountYourNumvber(QString SeuNumero, QString ap, QString tow );
      QString ComposeFileName(QString strDir, QString FileName, QString Extension, int MaxLen);
};


#endif
