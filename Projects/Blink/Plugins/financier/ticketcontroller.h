#ifndef TICKETCONTROLLER_H
#define TICKETCONTROLLER_H

#include "ticket.h"

#include "buildtkt.h"
#include "dweller.h"
#include "extratx.h"



class TicketController
{
public:
    TicketController();
    ~TicketController();

    bool BuildTicketCond(int id=0 );
    bool BuildTicketExtra( extratx *pTx );
    void OpenRemDir();
    void OpenPDF();
    void SendEmail();
    bool doShipp(QString dir="", QString filename="",BBO_TYPE type=tpTxCond, BBOL_STATUS status=stCreated);
    bool doPrint(BBO_TYPE type, BBOL_STATUS status, ticket *ptkt=0);
    bool Edit(int id);
    bool Remove(int id, QString strMsgText );
    bool ProcessRetList(QList<BankTicket*> *list);
    static bool UpdateTickets(QList<BankTicket*> *list);
    bool SendEmail(int id, bool bSilent=false );
    bool ReportExaro( QSqlQueryModel *model, QString reportTitle);
    bool SendToAll();
    bool ManualPayment(int TicketId);
private:
    BuildTkt *g_tkt;
    ticketconfig *m_pTktConfig;

    bool InitAcbr();
    bool doPrepare(BBO_TYPE type, BBOL_STATUS status);

    void Send(Dweller *pDweller);
    bool BuildTicket( DwellerList *dlist,
                      QDate date,
                      int type,
                      QString Obs,
                      int ExtraTxId=0,
                      double dValue=0,
                      double dDiscount =0);
};

#endif // TICKETCONTROLLER_H
