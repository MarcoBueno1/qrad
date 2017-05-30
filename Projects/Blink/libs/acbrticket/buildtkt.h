#ifndef _BUILDTKT_H
#define _BUILDTKT_H

#include <QObject>
#include  "ticketconfig.h"
#include  "bankaccount.h"
#include  "bankmodel.h"
#include  "maincompanyV2.h"
#include  "dweller.h"
#include  <QFileSystemWatcher>

#define TKT_DEFAULT_PDF_PATH "c:\\TEMP"
class BuildTkt : QObject
{
public:
      BuildTkt();
      virtual ~BuildTkt(){}
      
       bool Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount);
       bool Print(bool bPrinter=false, QString strPath=TKT_DEFAULT_PDF_PATH );
       bool BuildShipping(QString strDir, QString FileName );
       bool AddTicket(Dweller *pDweller, QString strValue, QDate dtVencto);

public slots:
       void DirModified(QString dir );

private:
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
