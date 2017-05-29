#ifndef _BUILDTKT_H
#define _BUILDTKT_H

#include  "ticketconfig.h"
#include  "bankaccount.h"
#include  "bankmodel.h"
#include  "maincompanyV2.h"
#include  "dweller.h"

#define TKT_DEFAULT_PDF_PATH "c:\\TEMP"
class BuildTkt
{
public:
      BuildTkt(){}
      virtual ~BuildTkt(){}
      
       bool Init(MainCompany *pCompany, ticketconfig *pTktConfig, BankModel *pBank, bankaccount *pAccount);
       bool Print(bool bPrinter=false, QString strPath=TKT_DEFAULT_PDF_PATH );
       bool BuildShipping(QString strDir, QString FileName );
       bool AddTicket(Dweller *pDweller, QString strValue, QDate dtVencto);

private:
      int m_ShippNumber;
      int m_TktCount;
       bool Send(QString cmd);
      MainCompany *m_pCompany;
      ticketconfig *m_pTktConfig;
      BankModel *m_pBank;
      bankaccount *m_pAccount;
};


#endif
